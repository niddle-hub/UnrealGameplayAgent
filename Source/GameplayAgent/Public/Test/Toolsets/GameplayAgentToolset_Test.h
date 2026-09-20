// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/GameplayAgentToolset.h"
#include "GameplayAgentToolset_Test.generated.h"

UCLASS()
class GAMEPLAYAGENT_API UGameplayAgentToolset_Test : public UGameplayAgentToolset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(ToolTip="Greets the user."))
	void Greet(UObject* ContextObject);
	
	UFUNCTION(BlueprintCallable, meta=(ToolTip="Evaluates a numerical equation from string."))
	void Calculate(UObject* ContextObject, const FString& Str);
	
	UFUNCTION(BlueprintCallable, meta=(ToolTip="Spawns a cube."))
	void SpawnCube(UObject* ContextObject, FTransform Transform);

	UFUNCTION(BlueprintCallable)
	static void PrintToolsetMetadata_Test(UGameplayAgentToolset_Test* ToolsetObject);
};
