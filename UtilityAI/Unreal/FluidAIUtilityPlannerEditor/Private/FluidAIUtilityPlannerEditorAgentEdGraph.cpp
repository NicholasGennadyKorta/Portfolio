// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAIUtilityPlannerEditorAgentEdGraph.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Node Settings

void UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings::AllocateDefaultPins()
{
}

FLinearColor UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings::GetNodeTitleColor() const
{
	return FColor::FromHex("#00C853"); //Custom Green
}

FText UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Settings");
}

FText UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings::GetTooltipText() const
{
	return FText::FromString("");
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings::CanUserDeleteNode() const
{
	return false;
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings::CanDuplicateNode() const
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Node Utility Model

void UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::AllocateDefaultPins()
{
}

FLinearColor UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::GetNodeTitleColor() const
{
	return FColor::FromHex("#00E5FF"); //Blue Light
}

FText UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Utility Model");
}

FText UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::GetTooltipText() const
{
	return FText::FromString("");
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::CanUserDeleteNode() const
{
	return false;
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::CanDuplicateNode() const
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Node Utility Sub Model

void UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::AllocateDefaultPins()
{
}

FLinearColor UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::GetNodeTitleColor() const
{
	return FColor::FromHex("#006064"); //Blue Dark
}

FText UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Utility SubModel : " + Name.ToString());
}

FText UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::GetTooltipText() const
{
	return FText::FromString("");
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::CanUserDeleteNode() const
{
	return true;
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::CanDuplicateNode() const
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Schema Action Create Node

UEdGraphNode* FFluidAIUtilityPlannerEditorAgentEdGraphSchemaActionCreateNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (NodeTypeName == "utilitySubModel")
	{
		const auto Node = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel*)UFluidAIUtilityPlannerEditorAgentEdGraph::CreateEdGraphNode(ParentGraph,NodeTypeName,Location.X,Location.Y);
		return Node;
	}
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Schema

EGraphType UFluidAIUtilityPlannerEditorAgentEdGraphSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_StateMachine;
}

void UFluidAIUtilityPlannerEditorAgentEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	//Utility Archetype
	const auto ActionUtilityInput = new FFluidAIUtilityPlannerEditorAgentEdGraphSchemaActionCreateNode(FText::FromString(""), FText::FromString("Utility SubModel"), FText::FromString(""), 0);
	ActionUtilityInput->NodeTypeName = "utilitySubModel";
	const TSharedPtr<FFluidAIUtilityPlannerEditorAgentEdGraphSchemaActionCreateNode> ActionUtilityInputPtr(ActionUtilityInput);
	ContextMenuBuilder.AddAction(ActionUtilityInputPtr);
}

void UFluidAIUtilityPlannerEditorAgentEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	//Node Actions
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("Node Actions", FText::FromString("Node Actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		//Section.AddMenuEntry(FGenericCommands::Get().Cut);
		//Section.AddMenuEntry(FGenericCommands::Get().Copy);
		//Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
		//Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}
}

const FPinConnectionResponse UFluidAIUtilityPlannerEditorAgentEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	FPinConnectionResponse R;

	//Make Sure Node Is Not Same | Do Not Allow For Same Directions
	if (A->GetOwningNode() == B->GetOwningNode() || A->Direction == B->Direction)
		R.Response = ECanCreateConnectionResponse::CONNECT_RESPONSE_DISALLOW;

	//Don't Allow More Than One Connection
	for (int i = 0; i < A->GetOwningNode()->GetAllPins().Num(); i++)
		if (A->GetOwningNode()->GetAllPins()[i]->GetOwningNode() == B->GetOwningNode())
			R.Response = ECanCreateConnectionResponse::CONNECT_RESPONSE_DISALLOW;

	return R;
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	return false;
}

FConnectionDrawingPolicy* UFluidAIUtilityPlannerEditorAgentEdGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	return nullptr;
}

FLinearColor UFluidAIUtilityPlannerEditorAgentEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor();
}

void UFluidAIUtilityPlannerEditorAgentEdGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	Super::BreakNodeLinks(TargetNode);
}

void UFluidAIUtilityPlannerEditorAgentEdGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UFluidAIUtilityPlannerEditorAgentEdGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

UEdGraphPin* UFluidAIUtilityPlannerEditorAgentEdGraphSchema::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	return nullptr;
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphSchema::SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	return false;
}

bool UFluidAIUtilityPlannerEditorAgentEdGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return false;
}

int32 UFluidAIUtilityPlannerEditorAgentEdGraphSchema::GetCurrentVisualizationCacheID() const
{
	return int32();
}

void UFluidAIUtilityPlannerEditorAgentEdGraphSchema::ForceVisualizationCacheClear() const
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph

 UEdGraphNode* UFluidAIUtilityPlannerEditorAgentEdGraph::CreateEdGraphNode(UEdGraph* EdGraph, FName TypeName, float PositionX, float PositionY)
{
	UEdGraphNode* Node = nullptr;
	if (TypeName == "settings")
		Node = NewObject<UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings>(EdGraph);
	if (TypeName == "utilityModel")
		Node = NewObject<UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel>(EdGraph);
	else if (TypeName == "utilitySubModel")
	{
		Node = NewObject<UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel>(EdGraph);
		((UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel*)Node)->EditorCurve = NewObject<UFluidAIUtilityPlannerEditorCurve>(EdGraph,UFluidAIUtilityPlannerEditorCurve::StaticClass());
	}
	if (Node == nullptr)
		return nullptr;
	
	Node->Rename(NULL, EdGraph);
	Node->CreateNewGuid();
	Node->SnapToGrid(10);
	Node->AllocateDefaultPins();
	Node->NodeWidth = 150;
	Node->NodeHeight = 50;
	Node->NodePosX = PositionX;
	Node->NodePosY = PositionY;
	EdGraph->AddNode(Node);
	return Node;
}

void UFluidAIUtilityPlannerEditorAgentEdGraph::ApplyUtilityModelNode()
{
	//Get Utility Model Inputs
	TArray<FName> UtilityModelInputs;
	for (int i = 0; i < Nodes.Num(); i++)
	{
		//Utility Model
		if (Nodes[i]->IsA(UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::StaticClass()))
		{
			UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel* Node = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel*)Nodes[i];
			UtilityModelInputs = Node->Inputs;
			break;
		}
	}
	
	//Apply Utility Model To Utility Sub Models
	for (int i = 0; i < Nodes.Num(); i++)
	{
		//Sub Model
		if (Nodes[i]->IsA(UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::StaticClass()))
		{
			//Get Node
			UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel* Node = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel*)Nodes[i];
			
			//Skip If Editor Curve Is Null Or Invalid
			if (Node->EditorCurve == nullptr || Node->EditorCurve->CurveNames.Num() != Node->EditorCurve->CurveRichCurves.Num())
				continue;
		
			//Map Old Curves
			auto UtilitySubModelCurveNames = Node->EditorCurve->CurveNames;
			auto UtilitySubModelRichCurves = Node->EditorCurve->CurveRichCurves;
			TMap<FName,FRichCurve> UtilitySubModelCurves;
			for (int j = 0; j < UtilitySubModelCurveNames.Num(); j++)
				UtilitySubModelCurves.Add(UtilitySubModelCurveNames[j],UtilitySubModelRichCurves[j]);

			//Empty Graph Node Utility Sub Model Curves
			Node->EditorCurve->CurveNames.Empty();
			Node->EditorCurve->CurveRichCurves.Empty();

			//Add In New Graph Node Utility Sub Model Curves
			for (int j = 0; j < UtilityModelInputs.Num(); j++)
			{
				//Add Old Curve Back In Since Its Still Being Used
				if (UtilitySubModelCurves.Contains(UtilityModelInputs[j]))
				{
					Node->EditorCurve->CurveNames.Add(UtilityModelInputs[j]);
					Node->EditorCurve->CurveRichCurves.Add(UtilitySubModelCurves[UtilityModelInputs[j]]);
				}

				//Add New Curve Since Was Not Being Used Previously
				else
				{
					Node->EditorCurve->CurveNames.Add(UtilityModelInputs[j]);
					Node->EditorCurve->CurveRichCurves.Add(FRichCurve());
				}
			}
		}
	}
}

void UFluidAIUtilityPlannerEditorAgentEdGraph::LoadFromAsset()
{
	//Settings
	const auto GraphNodeAgentSettings = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings*)CreateEdGraphNode(this,"settings",Asset->Settings.NodePosition.X,Asset->Settings.NodePosition.Y);
	GraphNodeAgentSettings->Settings = Asset->Settings;
	
	//Agent Model
	const auto GraphNodeAgentUtilityModel = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel*)CreateEdGraphNode(this,"utilityModel",Asset->AgentUtilityModelNodePosition.X,Asset->AgentUtilityModelNodePosition.Y);
	GraphNodeAgentUtilityModel->Inputs = Asset->AgentUtilityModelInputNames;
	
	//Agent Utility Sub Models
	for (int i = 0; i < Asset->AgentUtilitySubModels.Num(); i++)
	{
		auto Node = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel*)CreateEdGraphNode(this,"utilitySubModel",Asset->AgentUtilitySubModels[i].NodePosition.X,Asset->AgentUtilitySubModels[i].NodePosition.Y);
		Node->Name = Asset->AgentUtilitySubModels[i].Name;
		Node->EditorCurve->CurveRichCurves = Asset->AgentUtilitySubModels[i].Curves;
		Node->EditorCurve->CurveNames = Asset->AgentUtilitySubModels[i].InputNames;
		Node->OutputNameModifiers = Asset->AgentUtilitySubModels[i].OutputNameModifiers;
		Node->Tags = Asset->AgentUtilitySubModels[i].Tags;
	}

	//("wonder","flee","drink","eat","sleep")
}

void UFluidAIUtilityPlannerEditorAgentEdGraph::SaveToAsset()
{
	//Agent Utility Archetypes
	Asset->AgentUtilitySubModels.Empty();
	for (int i = 0; i < Nodes.Num(); i++)
	{
		//Settings
		if (Nodes[i]->IsA(UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings::StaticClass()))
		{
			UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings* Node = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentSettings*)Nodes[i];
			Asset->Settings = Node->Settings;
			Asset->Settings.NodePosition = FVector2D(Node->NodePosX,Node->NodePosY);
		}
		
		//Utility Model
		if (Nodes[i]->IsA(UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel::StaticClass()))
		{
			UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel* Node = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilityModel*)Nodes[i];
			Asset->AgentUtilityModelNodePosition = FVector2D(Node->NodePosX,Node->NodePosY);
			Asset->AgentUtilityModelInputNames = Node->Inputs;
		}

		//Sub Model
		else if (Nodes[i]->IsA(UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::StaticClass()))
		{
			UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel* Node = (UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel*)Nodes[i];
			auto GraphNodeUtilitySubModel = FFluidAIUtilityPlannerAgentUtilitySubModel();
			GraphNodeUtilitySubModel.Name = Node->Name;
			GraphNodeUtilitySubModel.Curves = Node->EditorCurve->CurveRichCurves;
			GraphNodeUtilitySubModel.InputNames = Node->EditorCurve->CurveNames;
			GraphNodeUtilitySubModel.OutputNameModifiers = Node->OutputNameModifiers;
			GraphNodeUtilitySubModel.Tags = Node->Tags;
			GraphNodeUtilitySubModel.NodePosition = FVector2D(Node->NodePosX,Node->NodePosY);
			Asset->AgentUtilitySubModels.Add(GraphNodeUtilitySubModel);
		}
	}
	
	//Save
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Asset->GetPackage());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, false);
}

void UFluidAIUtilityPlannerEditorAgentEdGraph::OnCommandDeleteNodes(TSharedPtr<SGraphEditor> GraphEditor)
{
	//Set To Modify
	Modify();
	
	//Get Selected Nodes
	TArray<UEdGraphNode*> SelectedNodes;
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(GraphEditor->GetSelectedNodes()); NodeIt; ++NodeIt)
		if (Cast<UEdGraphNode>(*NodeIt)->CanUserDeleteNode())
			SelectedNodes.Add(Cast<UEdGraphNode>(*NodeIt));
	if (SelectedNodes.Num() == 0)
		return;

	//Clear Selected Nodes From Graph Editor
	GraphEditor->ClearSelectionSet();
	
	//Delete Selected Nodes
	for (int i = 0; i < SelectedNodes.Num(); i++)
	{
		//We Can Not Delete Node
		if (!SelectedNodes[i]->CanUserDeleteNode())
			continue;
		
		//Delete Node
		SelectedNodes[i]->GetSchema()->BreakNodeLinks(*SelectedNodes[i]);
		SelectedNodes[i]->Modify();
		SelectedNodes[i]->DestroyNode();
	}
}