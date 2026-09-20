#include "GameplayAgentEditor.h"
#include "GameplayAgentToolsetExporter.cpp"

#define LOCTEXT_NAMESPACE "FGameplayAgentEditorModule"

void FGameplayAgentEditorModule::StartupModule()
{
	ToolsetExport();
}

void FGameplayAgentEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGameplayAgentEditorModule, GameplayAgentEditor)