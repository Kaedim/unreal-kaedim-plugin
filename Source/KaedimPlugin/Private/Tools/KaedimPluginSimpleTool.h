// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include <string>
#include <vector>
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/SingleClickTool.h"
#include "Components/VerticalBox.h"
#include "KaedimPluginSimpleTool.generated.h"

/**
 * Builder for UKaedimPluginSimpleTool
 */
UCLASS()
class KAEDIMPLUGIN_API UKaedimPluginSimpleToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};



/**
 * Settings UObject for UKaedimPluginSimpleTool. This UClass inherits from UInteractiveToolPropertySet,
 * which provides an OnModified delegate that the Tool will listen to for changes in property values.
 */
UCLASS(Transient)
class KAEDIMPLUGIN_API UKaedimPluginSimpleToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()
public:
	UKaedimPluginSimpleToolProperties();
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = Assets)
	void DownloadAssets();

	void DownloadAsset(const FString& name, const FString& FBXUrl);


	UVerticalBox* ButtonContainer = nullptr;

	UPROPERTY(EditAnywhere, Category = Keys, meta = (DisplayName = "Dev ID"))
	FText DevID;
	UPROPERTY(EditAnywhere, Category = Keys, meta = (DisplayName = "API Key"))
	FText APIKey;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = Keys)
	void RegisterKeys();
	std::string jwt;

	//UFUNCTION(BlueprintCallable, CallInEditor, Category = Assets)
	//void testAddButton();
private:
	void AddLoadAssetButton(const FString& name, const FString& ObjUrl);
	void ProcessAssets(const FString& JsonData);

};





UCLASS()
class KAEDIMPLUGIN_API UKaedimPluginSimpleTool : public USingleClickTool
{
	GENERATED_BODY()

public:
	UKaedimPluginSimpleTool();

	virtual void SetWorld(UWorld* World);

	virtual void Setup() override;

protected:
	UPROPERTY()
	TObjectPtr<UKaedimPluginSimpleToolProperties> Properties;



protected:
	/** target World we will raycast into to find actors */
	UWorld* TargetWorld;
};