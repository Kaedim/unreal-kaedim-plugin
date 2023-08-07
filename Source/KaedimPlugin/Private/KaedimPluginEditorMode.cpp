// Copyright Epic Games, Inc. All Rights Reserved.

#include "KaedimPluginEditorMode.h"
#include "KaedimPluginEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "KaedimPluginEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/KaedimPluginSimpleTool.h"
#include "Tools/KaedimPluginInteractiveTool.h"

// step 2: register a ToolBuilder in FKaedimPluginEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "KaedimPluginEditorMode"

const FEditorModeID UKaedimPluginEditorMode::EM_KaedimPluginEditorModeId = TEXT("EM_KaedimPluginEditorMode");

FString UKaedimPluginEditorMode::SimpleToolName = TEXT("KaedimPlugin_RegisterKeys");


UKaedimPluginEditorMode::UKaedimPluginEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UKaedimPluginEditorMode::EM_KaedimPluginEditorModeId,
		LOCTEXT("ModeName", "Kaedim Plugin"),
		FSlateIcon(),
		true);
}


UKaedimPluginEditorMode::~UKaedimPluginEditorMode()
{
}


void UKaedimPluginEditorMode::ActorSelectionChangeNotify()
{
}

void UKaedimPluginEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FKaedimPluginEditorModeCommands& SampleToolCommands = FKaedimPluginEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UKaedimPluginSimpleToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UKaedimPluginEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FKaedimPluginEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UKaedimPluginEditorMode::GetModeCommands() const
{
	return FKaedimPluginEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
