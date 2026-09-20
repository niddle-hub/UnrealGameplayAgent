// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Components/GameplayAgentComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/SaveGame.h"
#include "Core/Data/GameplayAgentIdentity.h"
#include "Core/Data/GameplayAgentMemory.h"
#include "Core/Data/GameplayAgentToolset.h"
#include "Core/Settings/GameplayAgentSettings.h"
#include "Kismet/GameplayStatics.h"

UGameplayAgentComponent::UGameplayAgentComponent()
{
}

void UGameplayAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadAgentIdentity();
}

void UGameplayAgentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IdentityLoadHandle.IsValid())
	{
		IdentityLoadHandle->CancelHandle();
		IdentityLoadHandle.Reset();
	}

	if (ToolsetLoadHandle.IsValid())
	{
		ToolsetLoadHandle->CancelHandle();
		ToolsetLoadHandle.Reset();
	}

	Super::EndPlay(EndPlayReason);
}

void UGameplayAgentComponent::LoadAgentIdentity()
{
	if (AgentIdentity)
	{
		// Already loaded, just re-broadcast for callers that requested it explicitly.
		OnIdentityLoaded.Broadcast(AgentIdentity);
		return;
	}

	if (!AgentIdentityID.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: AgentIdentityID is not set, skipping identity load."), *GetName());
		return;
	}

	UAssetManager& AssetManager = UAssetManager::Get();

	// Fast path: already in memory (e.g. preloaded or shared with another agent).
	if (UGameplayAgentIdentity* Loaded = AssetManager.GetPrimaryAssetObject<UGameplayAgentIdentity>(AgentIdentityID))
	{
		HandleIdentityLoadedFrom(Loaded);
		return;
	}

	// Async path: type must be registered in the Asset Manager to resolve.
	IdentityLoadHandle = AssetManager.LoadPrimaryAsset(
		AgentIdentityID,
		TArray<FName>(),
		FStreamableDelegate::CreateUObject(this, &UGameplayAgentComponent::HandleIdentityLoaded));

	if (!IdentityLoadHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to start async load of identity '%s' (is the type registered in the Asset Manager settings?)."), *GetName(), *AgentIdentityID.ToString());
	}
}

void UGameplayAgentComponent::HandleIdentityLoaded()
{
	if (UGameplayAgentIdentity* Loaded = UAssetManager::Get().GetPrimaryAssetObject<UGameplayAgentIdentity>(AgentIdentityID))
	{
		HandleIdentityLoadedFrom(Loaded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Identity load completed but '%s' could not be resolved."), *GetName(), *AgentIdentityID.ToString());
	}
}

void UGameplayAgentComponent::HandleIdentityLoadedFrom(UGameplayAgentIdentity* Loaded)
{
	AgentIdentity = Loaded;
	IdentityLoadHandle.Reset();
	OnIdentityLoaded.Broadcast(AgentIdentity);

	// Identity resolves everything else: memory slot name and toolset scope.
	LoadAgentMemory();
	LoadAgentToolset();
}

void UGameplayAgentComponent::LoadAgentMemory()
{
	if (!AgentIdentity)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Cannot load memory before identity is loaded."), *GetName());
		return;
	}

	if (AgentMemory)
	{
		OnMemoryLoaded.Broadcast(AgentMemory);
		return;
	}

	if (AgentIdentity->AgentMemorySlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Identity '%s' has no memory slot name, memory is disabled for this agent."), *GetName(), *AgentIdentity->AgentFriendlyName);
		return;
	}

	// Save-game slots are small and local-disk, synchronous LoadGameFromSlot avoids a one-frame gap for core agent state.
	if (USaveGame* Slot = UGameplayStatics::LoadGameFromSlot(AgentIdentity->AgentMemorySlotName, 0))
	{
		AgentMemory = Cast<UGameplayAgentMemory>(Slot);
	}

	if (AgentMemory)
	{
		OnMemoryLoaded.Broadcast(AgentMemory);
	}
	else
	{
		// No save yet (first run) - start fresh. Saving later persists it via SaveGameToSlot.
		AgentMemory = NewObject<UGameplayAgentMemory>(this);
		UE_LOG(LogTemp, Log, TEXT("%s: No existing memory in slot '%s', created fresh memory."), *GetName(), *AgentIdentity->AgentMemorySlotName);
		OnMemoryLoaded.Broadcast(AgentMemory);
	}
}

void UGameplayAgentComponent::LoadAgentToolset()
{
	if (!AgentIdentity)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Cannot load toolset before identity is loaded."), *GetName());
		return;
	}

	if (AgentToolset)
	{
		OnToolsetLoaded.Broadcast(AgentToolset);
		return;
	}

	// Decide the toolset class based on the scope enum: Global -> from Developer Settings, Specified -> from the identity.
	TSoftClassPtr<UGameplayAgentToolset> ToolsetClassPtr;

	switch (AgentIdentity->ToolsetScope)
	{
	case EGameplayAgentToolsetScope::Global:
		ToolsetClassPtr = GetDefault<UGameplayAgentSettings>()->GlobalAgentToolset;
		break;

	case EGameplayAgentToolsetScope::Specified:
		ToolsetClassPtr = AgentIdentity->ToolsetOverride;
		break;

	default:
		ensureMsgf(false, TEXT("%s: Unhandled toolset scope %d."), *GetName(), static_cast<int32>(AgentIdentity->ToolsetScope));
		return;
	}

	UClass* ToolsetClass = ToolsetClassPtr.Get();
	if (ToolsetClass)
	{
		AgentToolset = NewObject<UGameplayAgentToolset>(this, ToolsetClass);
		OnToolsetLoaded.Broadcast(AgentToolset);
		return;
	}

	// Async path: class not in memory yet, stream it in and spawn the instance on completion.
	const FSoftObjectPath ToolsetClassPath = ToolsetClassPtr.ToSoftObjectPath();
	if (!ToolsetClassPath.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Toolset scope is '%s' but no toolset class is configured (check the identity asset or Gameplay Agent settings)."), *GetName(), *UEnum::GetValueAsString(AgentIdentity->ToolsetScope));
		return;
	}

	ToolsetLoadHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		ToolsetClassPath,
		FStreamableDelegate::CreateUObject(this, &UGameplayAgentComponent::HandleToolsetClassLoaded));
}

void UGameplayAgentComponent::HandleToolsetClassLoaded()
{
	ToolsetLoadHandle.Reset();

	UClass* ToolsetClass = nullptr;
	switch (AgentIdentity->ToolsetScope)
	{
	case EGameplayAgentToolsetScope::Global:
		ToolsetClass = GetDefault<UGameplayAgentSettings>()->GlobalAgentToolset.Get();
		break;

	case EGameplayAgentToolsetScope::Specified:
		ToolsetClass = AgentIdentity->ToolsetOverride.Get();
		break;

	default:
		return;
	}

	if (!ToolsetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Toolset class load finished but the class could not be resolved."), *GetName());
		return;
	}

	AgentToolset = NewObject<UGameplayAgentToolset>(this, ToolsetClass);
	OnToolsetLoaded.Broadcast(AgentToolset);
}
