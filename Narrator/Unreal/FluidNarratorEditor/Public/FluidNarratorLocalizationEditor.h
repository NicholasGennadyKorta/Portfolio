// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorRuntime.h"
#include "FluidNarratorEditor.h"
#include "HAL/Runnable.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "FileHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Editor/EditorEngine.h"
#include "StatusBarSubsystem.h"
#include "Styling/ToolBarStyle.h"
#include "Framework/Commands/UIAction.h"
#include "UnrealEd.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

/**
 * 
 */

class UFluidNarratorDeveloperSettings;

//Logging
DECLARE_LOG_CATEGORY_EXTERN(LogFluidNarratorLocalizationEditor, Log, All);

//Fluid Narrator Screenplay Editor
class FluidNarratorLocalizationEditor : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:

	SLATE_BEGIN_ARGS(FluidNarratorLocalizationEditor) { }
	SLATE_END_ARGS()

//Constructors
public:
	
	FluidNarratorLocalizationEditor();
	~FluidNarratorLocalizationEditor();

// FAssetEditorToolkit | FSerializableObject interface
public:
	
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;


//Register Tab Spawners
public:
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

//Initialize
public:
	
	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidNarratorLocalization* AssetIn);
	void InitializeToolbar(FToolBarBuilder& ToolbarBuilder);
	
//Spawn Tabs
public:

	TSharedRef<SDockTab> SpawnTabTexts(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabCultures(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabMiscAssetDetails(const FSpawnTabArgs& Args);
	
//Handles
public:
	
	void HandelAssetPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);

//Variables	
public:

	UFluidNarratorLocalization* Asset;
	FName AssetName;
	UFluidNarratorDeveloperSettings* NarratorDeveloperSettings;
	TSharedPtr<class SFluidNarratorLocalizationTextsSlateWidget> TextsWidget;
	TSharedPtr<class SFluidNarratorLocalizationCulturesSlateWidget> CulturesWidget;
	TSharedPtr<class IDetailsView> AssetMiscSettingsDetailsView;
	
};