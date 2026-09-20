// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LlamaDataTypes.h"
#include "Engine/DataAsset.h"
#include "GameplayAgentModelDefinition.generated.h"

UCLASS(Abstract)
class GAMEPLAYAGENT_API UGameplayAgentModelDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Model Name")
	FName ModelID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ShowOnlyInnerProperties))
	FLLMModelParams ModelParams;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
