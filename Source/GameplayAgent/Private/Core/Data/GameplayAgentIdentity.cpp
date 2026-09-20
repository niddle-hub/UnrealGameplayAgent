// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Data/GameplayAgentIdentity.h"

UGameplayAgentIdentity::UGameplayAgentIdentity()
{
	ToolsetScope = EGameplayAgentToolsetScope::Global;
}

FPrimaryAssetId UGameplayAgentIdentity::GetPrimaryAssetId() const
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return FPrimaryAssetId();
	}

	return FPrimaryAssetId(TEXT("GameplayAgentIdentity"), GetFName());
}
