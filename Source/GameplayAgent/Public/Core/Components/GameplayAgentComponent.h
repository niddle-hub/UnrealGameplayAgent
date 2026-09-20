// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAgentComponent.generated.h"

class UGameplayAgentIdentity;
class UGameplayAgentMemory;
class UGameplayAgentToolset;
struct FStreamableHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayAgentIdentityLoaded, UGameplayAgentIdentity*, LoadedIdentity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayAgentMemoryLoaded, UGameplayAgentMemory*, LoadedMemory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayAgentToolsetLoaded, UGameplayAgentToolset*, LoadedToolset);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYAGENT_API UGameplayAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameplayAgentComponent();

	UPROPERTY(BlueprintAssignable, Category= "Gameplay Agent")
	FOnGameplayAgentIdentityLoaded OnIdentityLoaded;

	UPROPERTY(BlueprintAssignable, Category= "Gameplay Agent")
	FOnGameplayAgentMemoryLoaded OnMemoryLoaded;

	UPROPERTY(BlueprintAssignable, Category= "Gameplay Agent")
	FOnGameplayAgentToolsetLoaded OnToolsetLoaded;

	UFUNCTION(BlueprintCallable, Category= "Gameplay Agent")
	void LoadAgentIdentity();

	UFUNCTION(BlueprintCallable, Category= "Gameplay Agent")
	void LoadAgentMemory();

	UFUNCTION(BlueprintCallable, Category= "Gameplay Agent")
	void LoadAgentToolset();

	UFUNCTION(BlueprintPure, Category= "Gameplay Agent")
	UGameplayAgentIdentity* GetAgentIdentity() const { return AgentIdentity; }

	UFUNCTION(BlueprintPure, Category= "Gameplay Agent")
	UGameplayAgentMemory* GetAgentMemory() const { return AgentMemory; }

	UFUNCTION(BlueprintPure, Category= "Gameplay Agent")
	UGameplayAgentToolset* GetAgentToolset() const { return AgentToolset; }

	UFUNCTION(BlueprintPure, Category= "Gameplay Agent")
	bool IsAgentIdentityLoaded() const { return AgentIdentity != nullptr; }

	UFUNCTION(BlueprintPure, Category= "Gameplay Agent")
	bool IsAgentMemoryLoaded() const { return AgentMemory != nullptr; }

	UFUNCTION(BlueprintPure, Category= "Gameplay Agent")
	bool IsAgentToolsetLoaded() const { return AgentToolset != nullptr; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Gameplay Agent", meta = (AllowedTypes = "GameplayAgentIdentity"))
	FPrimaryAssetId AgentIdentityID;

private:
	void HandleIdentityLoaded();
	void HandleIdentityLoadedFrom(UGameplayAgentIdentity* Loaded);
	void HandleToolsetClassLoaded();

	UPROPERTY()
	TObjectPtr<UGameplayAgentIdentity> AgentIdentity;
	
	UPROPERTY()
	TObjectPtr<UGameplayAgentMemory> AgentMemory;
	
	UPROPERTY()
	TObjectPtr<UGameplayAgentToolset> AgentToolset;

	TSharedPtr<FStreamableHandle> IdentityLoadHandle;
	TSharedPtr<FStreamableHandle> ToolsetLoadHandle;
};
