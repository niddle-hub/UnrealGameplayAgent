// Copyright Your Copyright

#include "Core/Data/GameplayAgentModelDefinition.h"
#include "LlamaUtility.h"
#include "HAL/FileManager.h"
#include "Misc/DataValidation.h"

FPrimaryAssetId UGameplayAgentModelDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType(TEXT("GameplayAgentModelDefinition")), ModelID);
}

#if WITH_EDITOR
EDataValidationResult UGameplayAgentModelDefinition::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	const FString ModelPath   = ModelParams.PathToModel.FilePath;
	const FString MmprojPath  = ModelParams.MmprojPath.FilePath;
	
	if (ModelPath.IsEmpty())
	{
		Context.AddError(FText::FromString(TEXT("PathToModel is empty. Set a valid .gguf model file (e.g. './model.gguf').")));
	}
	else
	{
		const FString FullModelPath = FLlamaPaths::ParsePathIntoFullPath(ModelPath);
		if (!IFileManager::Get().FileExists(*FullModelPath))
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("PathToModel file not found: \"%s\" (resolved to \"%s\"). Place the .gguf under Saved/Models or use an absolute path."), *ModelPath, *FullModelPath)));
		}
	}
	
	if (!MmprojPath.IsEmpty())
	{
		const FString FullMmprojPath = FLlamaPaths::ParsePathIntoFullPath(MmprojPath);
		if (!FullMmprojPath.Contains(TEXT("mmproj"), ESearchCase::IgnoreCase))
		{
			Context.AddWarning(FText::FromString(
				FString::Printf(TEXT("MmprojPath \"%s\" does not look like a multimodal projector (filename should contain 'mmproj'). Double-check you selected the right file."), *MmprojPath)));
		}
		if (!IFileManager::Get().FileExists(*FullMmprojPath))
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("MmprojPath file not found: \"%s\" (resolved to \"%s\"). If the projector is not used, leave MmprojPath empty."), *MmprojPath, *FullMmprojPath)));
		}
	}

	return Result;
}
#endif
