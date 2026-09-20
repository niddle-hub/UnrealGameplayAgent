#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Core/Data/GameplayAgentToolset.h"

namespace
{
	FString BuildToolsetJson(const UClass* Class)
	{
		if (!IsValid(Class))
		{
			return FString();
		}

		const TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
		Root->SetStringField(TEXT("class"), Class->GetName());

		TArray<TSharedPtr<FJsonValue>> Tools;

		for (TFieldIterator<UFunction> It(Class, EFieldIterationFlags::None); It; ++It)
		{
			const UFunction* Function = *It;

			TSharedRef<FJsonObject> Tool = MakeShared<FJsonObject>();
			Tool->SetStringField(TEXT("name"), Function->GetName());
			Tool->SetStringField(TEXT("tooltip"), Function->GetMetaData(TEXT("ToolTip")));

			TArray<TSharedPtr<FJsonValue>> Parameters;

			for (TFieldIterator<FProperty> PropIt(Function); PropIt; ++PropIt)
			{
				const FProperty* Property = *PropIt;

				if (!Property->HasAnyPropertyFlags(CPF_Parm))
				{
					continue;
				}

				TSharedRef<FJsonObject> Param = MakeShared<FJsonObject>();

				Param->SetStringField(TEXT("name"), Property->GetName());
				Param->SetStringField(TEXT("type"), Property->GetCPPType());
				Param->SetStringField(TEXT("tooltip"),Property->GetMetaData(TEXT("ToolTip")));

				const TCHAR* Direction =
					Property->HasAnyPropertyFlags(CPF_ReturnParm) ? TEXT("return") :
					Property->HasAnyPropertyFlags(CPF_OutParm)    ? TEXT("out") : TEXT("in");

				Param->SetStringField(TEXT("direction"), Direction);

				Parameters.Add(MakeShared<FJsonValueObject>(Param));
			}

			Tool->SetArrayField(TEXT("parameters"), MoveTemp(Parameters));
			Tools.Add(MakeShared<FJsonValueObject>(Tool));
		}

		Root->SetArrayField(TEXT("tools"), MoveTemp(Tools));

		FString Json;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
		
		FJsonSerializer::Serialize(Root, Writer);
		return Json;
	}

	void ToolsetExport()
	{
		const FString SaveDir = FPaths::ProjectContentDir() / TEXT("AgentToolsets");
		IFileManager::Get().MakeDirectory(*SaveDir, true);
		
		for (TObjectIterator<UClass> ClassIterator; ClassIterator; ++ClassIterator)
		{
			const UClass* Class = *ClassIterator;
			if (!Class || !Class->IsChildOf(UGameplayAgentToolset::StaticClass()) || Class->HasAnyClassFlags(CLASS_Abstract))
			{
				continue;
			}

			const FString ClassName = Class->GetName();
			const FString ClassExport = BuildToolsetJson(Class);
			if (ClassExport.IsEmpty())
			{
				continue;
			}
			
			const FString FilePath = SaveDir / (ClassName + TEXT(".json"));

			FFileHelper::SaveStringToFile(ClassExport, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		}
	}
}
