// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayAgentSettings.generated.h"

class UGameplayAgentCommandCatalog;
class UGameplayAgentLLMConfig;
class UGameplayAgentToolset;

UCLASS(config=Game, defaultconfig)
class GAMEPLAYAGENT_API UGameplayAgentSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, meta = (AllowedTypes = "GameplayAgentModelDefinition"), Category= "LLM")
	FPrimaryAssetId DefaultLanguageModel;
	
	UPROPERTY(Config, EditAnywhere, Category= "Gameplay Agent|Toolset")
	TSoftClassPtr<UGameplayAgentToolset> GlobalAgentToolset;
};
