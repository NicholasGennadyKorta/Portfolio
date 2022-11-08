// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNodeBase.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphUtilities.h"
#include "GraphEditorActions.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ToolMenu.h"
#include "Framework/Commands/GenericCommands.h"
#include "EdGraph/EdGraphSchema.h"
#include "FluidAIUtilityPlannerEditor.h"
#include "FluidAIUtilityPlannerSubsystem.h"
#include "FluidAIUtilityPlannerEditorAgentEdGraph.generated.h"

UCLASS()
class FLUIDAIUTILITYPLANNEREDITOR_API UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings : public UEdGraphNode
{
	GENERATED_BODY()

public:
	
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
		FFluidAIUtilityPlannerAgentSettings Settings;
};

UCLASS()
class FLUIDAIUTILITYPLANNEREDITOR_API UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel : public UEdGraphNode
{
	GENERATED_BODY()

public:
	
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;
	
	UPROPERTY(EditDefaultsOnly,Category="Properties")
		TArray<FName> Inputs;
};

UCLASS()
class FLUIDAIUTILITYPLANNEREDITOR_API UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel : public UEdGraphNode
{
	GENERATED_BODY()

public:
	
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
		FName Name;
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	    TMap<FName,FName> OutputNameModifiers;
	UPROPERTY(EditDefaultsOnly,Category="Properties")
		TArray<FName> Tags;
	UFluidAIUtilityPlannerEditorCurve* EditorCurve;
};

USTRUCT()
struct FLUIDAIUTILITYPLANNEREDITOR_API FFluidAIUtilityPlannerEditorAgentEdGraphSchemaActionCreateNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

public:
	FFluidAIUtilityPlannerEditorAgentEdGraphSchemaActionCreateNode() {}

	FFluidAIUtilityPlannerEditorAgentEdGraphSchemaActionCreateNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

	FName NodeTypeName;
};

UCLASS()
class FLUIDAIUTILITYPLANNEREDITOR_API UFluidAIUtilityPlannerEditorAgentEdGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
	virtual UEdGraphPin* DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const override;
	virtual bool SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;
};

UCLASS()
class FLUIDAIUTILITYPLANNEREDITOR_API UFluidAIUtilityPlannerEditorAgentEdGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	static UEdGraphNode* CreateEdGraphNode(UEdGraph* EdGraph, FName TypeName, float PositionX, float PositionY);
	void ApplyUtilityModelNode();
	void LoadFromAsset();
	void SaveToAsset();
	void OnCommandDeleteNodes(TSharedPtr<SGraphEditor> GraphEditor);
	
	UFluidAIUtilityPlannerAgent* Asset;
};

