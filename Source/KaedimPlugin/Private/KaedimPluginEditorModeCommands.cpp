// Copyright Epic Games, Inc. All Rights Reserved.

#include "KaedimPluginEditorModeCommands.h"
#include "KaedimPluginEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "KaedimPluginEditorModeCommands"

FKaedimPluginEditorModeCommands::FKaedimPluginEditorModeCommands()
	: TCommands<FKaedimPluginEditorModeCommands>("KaedimPluginEditorMode",
		NSLOCTEXT("KaedimPluginEditorMode", "KaedimPluginEditorModeCommands", "KaedimPlugin Editor Mode"),
		NAME_None,
		FEditorStyle::GetStyleSetName())
{
}

void FKaedimPluginEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Kaedim", "Opens the base panel for Kaedim", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FKaedimPluginEditorModeCommands::GetCommands()
{
	return FKaedimPluginEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
