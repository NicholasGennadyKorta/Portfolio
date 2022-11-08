// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailsView.h"
#include "DetailsViewObjectFilter.h"
#include "SCurveEditor.h"
#include "UObject/NoExportTypes.h"
#include "FileHelpers.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Curves/CurveBase.h"
#include "FluidAIUtilityPlannerEditor.h"
#include "FluidAIUtilityPlannerSubsystem.h"
#include "FluidAIUtilityPlannerEditorAgentEditor.generated.h"

class FGGAssetEditorToolbar;
class FAssetEditor_GenericGraph;
class FExtender;
class FToolBarBuilder;


UCLASS()
class FLUIDAIUTILITYPLANNEREDITOR_API UFluidAIUtilityPlannerEditorCurve : public UCurveBase
{
	GENERATED_BODY()
	
public:
	
	TArray<FRichCurve> CurveRichCurves;
	TArray<FName> CurveNames;
	
	virtual TArray<FRichCurveEditInfoConst> GetCurves() const override
	{
		TArray<FRichCurveEditInfoConst> RichCurveEditInfoConst;
		if (CurveRichCurves.Num() != CurveNames.Num()|| CurveRichCurves.IsEmpty() || CurveNames.IsEmpty())
			return RichCurveEditInfoConst;
		
		for (int i = 0; i < CurveRichCurves.Num(); i++)
			RichCurveEditInfoConst.Add(FRichCurveEditInfoConst(&CurveRichCurves[i],CurveNames[i]));
		return RichCurveEditInfoConst;
	}

	virtual TArray<FRichCurveEditInfo> GetCurves() override
	{
		TArray<FRichCurveEditInfo> RichCurveEditInfo;
		if (CurveRichCurves.Num() != CurveNames.Num() || CurveRichCurves.IsEmpty() || CurveNames.IsEmpty())
			return RichCurveEditInfo;
		
		for (int i = 0; i < CurveRichCurves.Num(); i++)
			RichCurveEditInfo.Add(FRichCurveEditInfo(&CurveRichCurves[i],CurveNames[i]));
		return RichCurveEditInfo;
	}
	
	virtual bool IsValidCurve(FRichCurveEditInfo CurveInfo) override
	{
		return CurveRichCurves.Num() == CurveNames.Num() && !CurveRichCurves.IsEmpty() && !CurveNames.IsEmpty();
	};
};

//Editor
class FLUIDAIUTILITYPLANNEREDITOR_API FluidAIUtilityPlannerEditorAgentEditor : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
	
public:
	FluidAIUtilityPlannerEditorAgentEditor();
	~FluidAIUtilityPlannerEditorAgentEditor();

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
	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidAIUtilityPlannerAgent* AssetIn);
	void InitializeGraphEditor();
	void InitializeCurveEditor();
	void InitializeDetailsView();
	void InitializeToolbar();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);

	//Spawn Tabs
	TSharedRef<SDockTab> SpawnTabGraphEditor(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabCurveEditor(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTabDetails(const FSpawnTabArgs& Args);

	//Input
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	//Graph Editor Commands
	bool CanDoCommand();
	void OnCommandDeleteNodes();
	
protected:
	UFluidAIUtilityPlannerAgent* Asset;
	FName AssetName;
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedPtr<SGraphEditor> GraphEditor;
	TSharedPtr<FUICommandList> GraphEditorCommands;
	class UFluidAIUtilityPlannerEditorAgentEdGraph* EdGraph;
	TSharedPtr<SCurveEditor> CurveEditor;
};