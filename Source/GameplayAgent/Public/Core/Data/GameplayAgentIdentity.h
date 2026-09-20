// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAgentToolset.h"
#include "Engine/DataAsset.h"
#include "GameplayAgentIdentity.generated.h"

class UGameplayAgentToolset;

UCLASS(Abstract, BlueprintType)
class GAMEPLAYAGENT_API UGameplayAgentIdentity : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGameplayAgentIdentity();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Gameplay Agent")
	FString AgentFriendlyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Gameplay Agent")
	FString AgentMemorySlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Gameplay Agent|Toolset")
	EGameplayAgentToolsetScope ToolsetScope;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="ToolsetScope==EGameplayAgentToolsetScope::Specified", EditConditionHides), Category= "Gameplay Agent|Toolset")
	TSoftClassPtr<UGameplayAgentToolset> ToolsetOverride;

protected:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
