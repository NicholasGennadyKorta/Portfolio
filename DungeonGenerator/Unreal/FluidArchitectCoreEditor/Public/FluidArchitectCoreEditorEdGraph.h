// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <FluidArchitectCoreRuntime/Public/FluidArchitectCoreSubsystem.h>
#include "FluidArchitectCoreEditor.h"
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
#include "FluidArchitectCoreEditorEdGraph.generated.h"

UCLASS()
class FLUIDARCHITECTCOREEDITOR_API UFluidArchitectCoreEditorEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UPROPERTY(Instanced, EditDefaultsOnly)
		UFluidArchitectCoreGraphNodeEvent* GraphNodeEvent;	

	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;
};

USTRUCT()
struct FLUIDARCHITECTCOREEDITOR_API FFluidArchitectCoreEditorEdGraphSchemaAction : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

public:
	FFluidArchitectCoreEditorEdGraphSchemaAction() {}

	FFluidArchitectCoreEditorEdGraphSchemaAction(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	TSubclassOf<UFluidArchitectCoreGraphNodeEvent> GraphNodeEventClass;
};

UCLASS()
class FLUIDARCHITECTCOREEDITOR_API UFluidArchitectCoreEditorEdGraphSchema : public UEdGraphSchema
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

	UFluidArchitectCoreProfile* Asset;
};

UCLASS()
class FLUIDARCHITECTCOREEDITOR_API UFluidArchitectCoreEditorEdGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	static UFluidArchitectCoreEditorEdGraphNode* CreateTemplateEdGraphNode(UEdGraph* EdGraph);
	
public:
	FName ModuleUID;
	FName ModuleName;
	
	void LoadFromAsset(UFluidArchitectCoreProfile* Asset);
	void SaveToAsset(UFluidArchitectCoreProfile* Asset);
	void OnCommandDeleteNodes(TSharedPtr<SGraphEditor> GraphEditor);
	void OnCommandCopyNodes(TSharedPtr<SGraphEditor> GraphEditor);
	void OnCommandCutNodes(TSharedPtr<SGraphEditor> GraphEditor);
	void OnCommandPasteNodes(TSharedPtr<SGraphEditor> GraphEditor);
	void OnCommandDuplicateNodes(TSharedPtr<SGraphEditor> GraphEditor);
};

