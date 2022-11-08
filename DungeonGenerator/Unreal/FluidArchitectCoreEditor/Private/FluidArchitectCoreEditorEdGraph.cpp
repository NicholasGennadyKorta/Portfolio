// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidArchitectCoreEditorEdGraph.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Node

void UFluidArchitectCoreEditorEdGraphNode::AllocateDefaultPins()
{;
	if (GraphNodeEvent != nullptr && Pins.Num() == 0)
	{
		CreatePin(EGPD_Input,FName(), FName(), FName());
		CreatePin(EGPD_Output, FName(), FName(), FName());
	}
}

FLinearColor UFluidArchitectCoreEditorEdGraphNode::GetNodeTitleColor() const
{
	if (GraphNodeEvent == nullptr)
			return FLinearColor::Black;
	return  GraphNodeEvent->GetColor();
}

FText UFluidArchitectCoreEditorEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (GraphNodeEvent == nullptr)
		return FText::FromString("NULL");
	return GraphNodeEvent->GetTitle();
}

FText UFluidArchitectCoreEditorEdGraphNode::GetTooltipText() const
{
	return FText::FromString("");
}

bool UFluidArchitectCoreEditorEdGraphNode::CanUserDeleteNode() const
{
	return true;
}

bool UFluidArchitectCoreEditorEdGraphNode::CanDuplicateNode() const
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Schema Action

UEdGraphNode* FFluidArchitectCoreEditorEdGraphSchemaAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UFluidArchitectCoreEditorEdGraphNode* Node = UFluidArchitectCoreEditorEdGraph::CreateTemplateEdGraphNode(ParentGraph);
	Node->GraphNodeEvent = NewObject<UFluidArchitectCoreGraphNodeEvent>(Node,GraphNodeEventClass);
	Node->AllocateDefaultPins();
	Node->NodePosX = Location.X;
	Node->NodePosY = Location.Y;
	ParentGraph->AddNode(Node);
	return Node;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph Schema

EGraphType UFluidArchitectCoreEditorEdGraphSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_StateMachine;
}

void UFluidArchitectCoreEditorEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	//Validate Asset
	if (Asset == nullptr)
		return;

	for (int i = 0; i < Asset->RegistredGraphNodeEventClasses.Num();i++)
	{
		auto Action = new FFluidArchitectCoreEditorEdGraphSchemaAction(FText::FromString(""), FText::FromString(Asset->RegistredGraphNodeEventClasses[i].GetDefaultObject()->GetSearchName().ToString()), FText::FromString(""), 0);
		Action->GraphNodeEventClass = Asset->RegistredGraphNodeEventClasses[i];
		TSharedPtr<FFluidArchitectCoreEditorEdGraphSchemaAction> NewEventNodeAction(Action);

		ContextMenuBuilder.AddAction(NewEventNodeAction);
	}
}

void UFluidArchitectCoreEditorEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	//Node Actions
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("Node Actions", FText::FromString("Node Actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		Section.AddMenuEntry(FGenericCommands::Get().Cut);
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
		Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}
}

const FPinConnectionResponse UFluidArchitectCoreEditorEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
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

bool UFluidArchitectCoreEditorEdGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	return false;
}

FConnectionDrawingPolicy* UFluidArchitectCoreEditorEdGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	return nullptr;
}

FLinearColor UFluidArchitectCoreEditorEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor();
}

void UFluidArchitectCoreEditorEdGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	Super::BreakNodeLinks(TargetNode);
}

void UFluidArchitectCoreEditorEdGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UFluidArchitectCoreEditorEdGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

UEdGraphPin* UFluidArchitectCoreEditorEdGraphSchema::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	return nullptr;
}

bool UFluidArchitectCoreEditorEdGraphSchema::SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	return false;
}

bool UFluidArchitectCoreEditorEdGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return false;
}

int32 UFluidArchitectCoreEditorEdGraphSchema::GetCurrentVisualizationCacheID() const
{
	return int32();
}

void UFluidArchitectCoreEditorEdGraphSchema::ForceVisualizationCacheClear() const
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Core Editor Graph

UFluidArchitectCoreEditorEdGraphNode* UFluidArchitectCoreEditorEdGraph::CreateTemplateEdGraphNode(UEdGraph* EdGraph)
{
	UFluidArchitectCoreEditorEdGraphNode* Node = NewObject<UFluidArchitectCoreEditorEdGraphNode>(EdGraph);
	Node->Rename(NULL, EdGraph);
	Node->CreateNewGuid();
	Node->SnapToGrid(10);
	Node->AllocateDefaultPins();
	Node->NodeWidth = 150;
	Node->NodeHeight = 50;
	return Node;
}

void UFluidArchitectCoreEditorEdGraph::LoadFromAsset(UFluidArchitectCoreProfile* Asset)
{
	//Validate Asset
	if (Asset == nullptr)
		return;
	
	//Get Node Data
	TArray<FFluidArchitectCoreGraphNode> NodeDatas;
	TArray<FFluidArchitectCoreGraphNodeConnection> NodeConnectionDatas;
	for (int i = 0; i < Asset->Modules.Num(); i++)
		if (Asset->Modules[i].UID == ModuleUID)
		{
			NodeDatas = Asset->Modules[i].Nodes;
			NodeConnectionDatas = Asset->Modules[i].NodeConnections;
		}

	//Write Node Data To EdGraphNodes
	for (int i = 0; i < NodeDatas.Num(); i++)
	{
		auto NewEDGraphNode = CreateTemplateEdGraphNode(this);
		NewEDGraphNode->NodeGuid = NodeDatas[i].Guid;
		NewEDGraphNode->NodePosX = NodeDatas[i].PosX;
		NewEDGraphNode->NodePosY = NodeDatas[i].PosY;
		NewEDGraphNode->GraphNodeEvent = NodeDatas[i].GraphNodeEvent;
		NewEDGraphNode->AllocateDefaultPins();
		AddNode(NewEDGraphNode);
	}

	//Write Node Data Connections To EdGraphNodes
	for (int i = 0; i < NodeConnectionDatas.Num(); i++)
	{
		UEdGraphNode* ConnectToEdGraphNode = nullptr;
		UEdGraphNode* ConnectFromEdGraphNode = nullptr;

		for (int j = 0; j < Nodes.Num(); j++)
		{
			if (Nodes[j]->NodeGuid == NodeConnectionDatas[i].ConnectionByUIDTo)
				ConnectToEdGraphNode = Nodes[j];
			if (Nodes[j]->NodeGuid == NodeConnectionDatas[i].ConnectionByUIDFrom)
				ConnectFromEdGraphNode = Nodes[j];
		}
		
		if (ConnectToEdGraphNode != nullptr && ConnectFromEdGraphNode != nullptr)
		{
			ConnectToEdGraphNode->AllocateDefaultPins();
			ConnectFromEdGraphNode->AllocateDefaultPins();
			auto Pin = ConnectFromEdGraphNode->GetPinAt(1);
			Pin->MakeLinkTo(ConnectToEdGraphNode->GetPinAt(0));
		}
	}

	//Notify Graph Changed Duh
	NotifyGraphChanged();
}

void UFluidArchitectCoreEditorEdGraph::SaveToAsset(UFluidArchitectCoreProfile* Asset)
{
	//Validate Asset
	if (Asset == nullptr)
		return;

	//Create Node Data
	TArray<FFluidArchitectCoreGraphNode> NodeDatas;
	TArray<FFluidArchitectCoreGraphNodeConnection> NodeConnectionDatas;
	
	//Write EdGraphNodes To Node Data
	for (int i = 0; i < Nodes.Num(); i++)
	{
		FFluidArchitectCoreGraphNode NodeData;
		NodeData.Guid = Nodes[i]->NodeGuid;
		NodeData.PosX = Nodes[i]->NodePosX;
		NodeData.PosY = Nodes[i]->NodePosY;
		NodeData.GraphNodeEvent = ((UFluidArchitectCoreEditorEdGraphNode*)Nodes[i])->GraphNodeEvent;
		NodeDatas.Add(NodeData);

		//Save Connections
		auto Pins = Nodes[i]->GetAllPins();
		for (int j = 0; j < Pins.Num(); j++)
			if (Pins[j]->Direction == EEdGraphPinDirection::EGPD_Output)
				for (int k = 0; k < Pins[j]->LinkedTo.Num(); k++)
				{
					FFluidArchitectCoreGraphNodeConnection NodeConnectionData;
					NodeConnectionData.ConnectionByUIDFrom = Nodes[i]->NodeGuid;
					NodeConnectionData.ConnectionByUIDTo = Pins[j]->LinkedTo[k]->GetOwningNode()->NodeGuid;
					NodeConnectionDatas.Add(NodeConnectionData);
				}
	}
	
	//Set Module Data
	for (int i = 0; i < Asset->Modules.Num(); i++)
		if (Asset->Modules[i].UID == ModuleUID)
		{
			Asset->Modules[i].Nodes = NodeDatas;
			Asset->Modules[i].NodeConnections = NodeConnectionDatas;
		}

	//Save
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Asset->GetPackage());
	PackagesToSave.Add(GetPackage());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, false);
}

void UFluidArchitectCoreEditorEdGraph::OnCommandDeleteNodes(TSharedPtr<SGraphEditor> GraphEditor)
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

void UFluidArchitectCoreEditorEdGraph::OnCommandCopyNodes(TSharedPtr<SGraphEditor> GraphEditor)
{
	//Get Selected Nodes
	TArray<UEdGraphNode*> SelectedNodes;
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(GraphEditor->GetSelectedNodes()); NodeIt; ++NodeIt)
			SelectedNodes.Add(Cast<UEdGraphNode>(*NodeIt));
	if (SelectedNodes.Num() == 0)
		return;

	//Prepare Copying Selected Nodes
	for (int i = 0; i < SelectedNodes.Num(); i++)
		SelectedNodes[i]->PrepareForCopying();

	FString ExportedText;
	FEdGraphUtilities::ExportNodesToText(GraphEditor->GetSelectedNodes(), ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

void UFluidArchitectCoreEditorEdGraph::OnCommandCutNodes(TSharedPtr<SGraphEditor> GraphEditor)
{
	OnCommandCopyNodes(GraphEditor);
	OnCommandDeleteNodes(GraphEditor);
}

void UFluidArchitectCoreEditorEdGraph::OnCommandPasteNodes(TSharedPtr<SGraphEditor> GraphEditor)
{
	//Nothing To Paste
	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
	if (!FEdGraphUtilities::CanImportNodesFromText(this, ClipboardContent))
		return;

	//Clear Selected Nodes From Graph Editor
	GraphEditor->ClearSelectionSet();
	
	//Set To Modify
	Modify();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);
	
	//Paste Nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(this, TextToImport, PastedNodes);

	//Get Average Node Position
	FVector2D AvgNodePosition(0.0f, 0.0f);
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}
	float InvNumNodes = 1.0f / float(PastedNodes.Num());
	AvgNodePosition.X *= InvNumNodes;
	AvgNodePosition.Y *= InvNumNodes;

	// Update Pasted Nodes
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		//Set Node Location
		UEdGraphNode* Node = *It;
		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + GraphEditor->GetPasteLocation().X;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + GraphEditor->GetPasteLocation().Y;

		//Snap To Grid | Set Selected | Create A New GUID (Different From Copied Node Guid)
		GraphEditor->SetNodeSelection(Node, true);
		Node->CreateNewGuid();
		Node->ReconstructNode();
	}

	// Update Graph
	GraphEditor->NotifyGraphChanged();
	PostEditChange();
	MarkPackageDirty();
}

void UFluidArchitectCoreEditorEdGraph::OnCommandDuplicateNodes(TSharedPtr<SGraphEditor> GraphEditor)
{
	OnCommandCopyNodes(GraphEditor);
	OnCommandPasteNodes(GraphEditor);
}

