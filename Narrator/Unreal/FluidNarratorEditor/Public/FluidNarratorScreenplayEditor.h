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
#include "PropertyCustomizationHelpers.h"
#include "Slate/SObjectWidget.h"
#include "UnrealEd.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

/**
 * 
 */

class UFluidNarratorDeveloperSettings;
class FluidNarratorScreenplayMarshaller;
class FluidNarratorScreenplayEditor;

//Logging
DECLARE_LOG_CATEGORY_EXTERN(LogFluidNarratorScreenplayEditor, Log, All);

//Fluid Narrator Screenplay Editor
class FluidNarratorScreenplayEditor : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:

	SLATE_BEGIN_ARGS(FluidNarratorScreenplayEditor) { }
	SLATE_END_ARGS()

//Constructors
public:
	
	FluidNarratorScreenplayEditor();
	~FluidNarratorScreenplayEditor();

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
	
	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidNarratorScreenplay* AssetIn);
	void InitializeToolbar(FToolBarBuilder& ToolbarBuilder);
	
//Spawn Tabs
public:
	
	TSharedRef<SDockTab> SpawnTabTextEditor(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabSimulatorData(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabSimulator(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabEditorSettingsDetails(const FSpawnTabArgs& Args);
	
//Handles
public:

	void HandelScreenplayStart();
	void HandelScreenplayStop();
	void HandelScreenplayTick();
	void HandelToolbarSimulatorButtonPressed();
	void HandelAssetPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);

//Variables	
public:
	UFluidNarratorScreenplay* Asset;
	FName AssetName;
	UFluidNarratorDeveloperSettings* NarratorDeveloperSettings;
	FluidNarratorScreenplayMarshaller* Marshaller;
	TSharedPtr<SEditableTextBox> ToolbarSimulatorCultureEditableTextBox;
	//TSharedPtr<SSingleProperty> ToolbarSimulatorLocalizationAssetPropertyView;
	TSharedPtr<SFluidNarratorScreenplayScriptViewWidget> ScriptViewWidget;
	TSharedPtr<SMultiLineEditableTextBox> TextEditorTextBoxLineNumbers;
	TSharedPtr<SMultiLineEditableTextBox> SimulatorVariablesTextBlock;
	TSharedPtr<SFluidNarratorScreenplaySimulatorSlateWidget> SimulatorWidget;
	TSharedPtr<IDetailsView> EditorSettingsDetailView;
	FluidNarratorScreenplaySystem ScreenplaySystem;
	UFluidNarratorLocalization* Localization;
	int ScreenplayTask;
};