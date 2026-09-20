// Fill out your copyright notice in the Description page of Project Settings.

#include "Test/Toolsets/GameplayAgentToolset_Test.h"
#include "Engine/StaticMeshActor.h"

void UGameplayAgentToolset_Test::Greet(UObject* ContextObject)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Hello User!");
}

void UGameplayAgentToolset_Test::Calculate(UObject* ContextObject, const FString& Str)
{
	float Result = 0;
	FMath::Eval(Str, Result);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%f"), Result));
}

void UGameplayAgentToolset_Test::SpawnCube(UObject* ContextObject, FTransform Transform)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (AStaticMeshActor* CubeActor = World->SpawnActor<AStaticMeshActor>(Transform.GetTranslation(), Transform.Rotator()))
		{
			constexpr const TCHAR* CubeMeshPath = TEXT("/Engine/BasicShapes/Cube.Cube");
			UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, CubeMeshPath);
			CubeActor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
		}
	}
}

void UGameplayAgentToolset_Test::PrintToolsetMetadata_Test(UGameplayAgentToolset_Test* ToolsetObject)
{
	for (TFieldIterator<UFunction> It(ToolsetObject->GetClass()); It; ++It)
	{
		UFunction* Function = *It;
		if (!Function || (Function->FunctionFlags & FUNC_BlueprintCallable) == 0 || Function->GetFName() == GET_FUNCTION_NAME_CHECKED(ThisClass, PrintToolsetMetadata_Test))
		{
			continue;
		}

		struct FFunctionParamInfo
		{
			FName Name;
			FString TypeName;
		};

		struct FFunctionReturnProperty
		{
			FName TypeName;
		};

		struct FFunctionInfo
		{
			FName Name;
			FString Description;
			TArray<FFunctionParamInfo> Params;
			TOptional<FFunctionReturnProperty> ReturnProperty;
		};
		
		FFunctionInfo FuncInfo;
		FuncInfo.Name = Function->GetFName();
		FuncInfo.Description = Function->GetMetaData(TEXT("tooltip"));

		FString ParamsOutStr;
		for (TFieldIterator<FProperty> ParamIt(Function); ParamIt && (ParamIt->PropertyFlags & CPF_Parm) && !(ParamIt->PropertyFlags & CPF_ReturnParm); ++ParamIt)
		{
			FFunctionParamInfo ParamInfo;
			ParamInfo.Name = ParamIt->GetFName();
			ParamInfo.TypeName = ParamIt->GetCPPType(); // not woring
			FuncInfo.Params.Add(MoveTemp(ParamInfo));

			ParamsOutStr += ParamInfo.TypeName + TEXT(" ") + ParamInfo.Name.ToString() + TEXT(", ");
		}

		if (FProperty* ReturnProperty = Function->GetReturnProperty())
		{
			FFunctionReturnProperty ReturnPropertyInfo;
			ReturnPropertyInfo.TypeName = ReturnProperty->GetFName();
			FuncInfo.ReturnProperty = ReturnPropertyInfo;
		}
		
		FString ReturnStr = FuncInfo.ReturnProperty ? FuncInfo.ReturnProperty->TypeName.ToString() : TEXT("void");

		UE_LOGFMT(LogTemp, Display, "Function: {name}, Description: {desc}, Params: {params}, Return: {ret}", FuncInfo.Name, FuncInfo.Description, ParamsOutStr, ReturnStr);
	}
}

