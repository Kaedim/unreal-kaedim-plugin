// Copyright Epic Games, Inc. All Rights Reserved.

#include "KaedimPluginModule.h"
#include "KaedimPluginEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "KaedimPluginModule"

void FKaedimPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FKaedimPluginEditorModeCommands::Register();
}

void FKaedimPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FKaedimPluginEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKaedimPluginModule, KaedimPluginEditorMode)