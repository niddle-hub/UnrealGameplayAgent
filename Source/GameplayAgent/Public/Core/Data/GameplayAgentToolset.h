// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayAgentToolset.generated.h"

UENUM(BlueprintType)
enum class EGameplayAgentToolsetScope : uint8
{
	/** One toolset shared by all agents */
	Global,
	/** Toolset defined explicitly per agent */
	Specified
};

UCLASS(DisplayName="Gameplay Agent Toolset", Blueprintable, BlueprintType, Abstract)
class GAMEPLAYAGENT_API UGameplayAgentToolset : public UObject
{
	GENERATED_BODY()
};
