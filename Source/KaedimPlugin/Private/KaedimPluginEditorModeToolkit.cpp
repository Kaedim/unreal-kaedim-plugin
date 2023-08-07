// Copyright Epic Games, Inc. All Rights Reserved.

#include "KaedimPluginEditorModeToolkit.h"
#include "KaedimPluginEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "KaedimPluginEditorModeToolkit"

FKaedimPluginEditorModeToolkit::FKaedimPluginEditorModeToolkit()
{
}

void FKaedimPluginEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FKaedimPluginEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FKaedimPluginEditorModeToolkit::GetToolkitFName() const
{
	return FName("KaedimPluginEditorMode");
}

FText FKaedimPluginEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "KaedimPluginEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
