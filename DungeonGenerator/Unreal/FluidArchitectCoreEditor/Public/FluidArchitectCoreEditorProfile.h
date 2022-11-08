// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <FluidArchitectCoreRuntime/Public/FluidArchitectCoreSubsystem.h>
#include "FluidArchitectCoreEditor.h"
#include "CoreMinimal.h"
#include "IDetailsView.h"
#include "UObject/NoExportTypes.h"
#include "FileHelpers.h"
#include "Kismet2/BlueprintEditorUtils.h"

class FGGAssetEditorToolbar;
class FAssetEditor_GenericGraph;
class FExtender;
class FToolBarBuilder;

//Editor
class FLUIDARCHITECTCOREEDITOR_API FluidArchitectCoreEditorProfile : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
	
public:
	FluidArchitectCoreEditorProfile();
	~FluidArchitectCoreEditorProfile();

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	// FAssetEditorToolkit | FSerializableObject interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
	//Initialize
	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidArchitectCoreProfile* AssetIn);
	void InitializeGraphEditors();
	void InitializePreviewViewports();
	void InitializeDetailsView();
	void InitializeToolbar();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);

	//Spawn Tabs
	TSharedRef<SDockTab> SpawnTabGraphEditor(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabPreviewViewports(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabDetails(const FSpawnTabArgs& Args);

	//Input
	void OnGenerate();
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	//Graph Editor Commands
	bool CanDoCommand();
	void OnCommandDeleteNodes();
	void OnCommandCopyNodes();
	void OnCommandCutNodes();
	void OnCommandPasteNodes();
	void OnCommandDuplicateNodes();
	
protected:
	UFluidArchitectCoreProfile* Asset;
	FName AssetName;
	TSharedPtr<class IDetailsView> DetailsView;
	TArray<TSharedPtr<SGraphEditor>> GraphEditors;
	TSharedPtr<FUICommandList> GraphEditorCommands;
	TArray<class UFluidArchitectCoreEditorEdGraph*> EdGraphs;
	TArray<TSharedPtr<class SFluidArchitectCorePreviewViewportWidget>> PreviewViewports;
	int CurrentGraphEditorToInitializeByIndex;
	int CurrentPreviewViewportsToInitializeByIndex;
};