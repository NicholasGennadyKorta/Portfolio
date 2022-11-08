// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAIUtilityPlannerEditorAgentEditor.h"

//#include "CurveEditor/Public/Views/SCurveEditorViewAbsolute.h"
//#include "CurveEditor/Public/Views/SCurveEditorViewStacked.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Property Identifiers

const FName CurveEditorID(TEXT("FluidAIUtilityPlannerAgentEditorCurveEditor"));
const FName GraphEditorID(TEXT("FluidAIUtilityPlannerAgentEditorGraphEditor"));
const FName DetailsID(TEXT("FluidAIUtilityPlannerAgentEditorDetails"));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Constructors

FluidAIUtilityPlannerEditorAgentEditor::FluidAIUtilityPlannerEditorAgentEditor()
{
}

FluidAIUtilityPlannerEditorAgentEditor::~FluidAIUtilityPlannerEditorAgentEditor()
{
	//UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Toolkit interface

void FluidAIUtilityPlannerEditorAgentEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	//AI Utility Planner Graph
	InTabManager->RegisterTabSpawner(GraphEditorID, FOnSpawnTab::CreateSP(this, &FluidAIUtilityPlannerEditorAgentEditor::SpawnTabGraphEditor))
			.SetDisplayName(FText::FromString("AI Utility Planner Graph"))
			.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), ""));

	//Curve Editor
	InTabManager->RegisterTabSpawner(CurveEditorID, FOnSpawnTab::CreateSP(this, &FluidAIUtilityPlannerEditorAgentEditor::SpawnTabCurveEditor))
			.SetDisplayName(FText::FromString("Curves"))
			.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), ""));
	
	//Details
	InTabManager->RegisterTabSpawner(DetailsID, FOnSpawnTab::CreateSP(this, &FluidAIUtilityPlannerEditorAgentEditor::SpawnTabDetails))
		.SetDisplayName(FText::FromString("Details"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GenericEditor.Tabs.Properties"));
}

void FluidAIUtilityPlannerEditorAgentEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(GraphEditorID);
	InTabManager->UnregisterTabSpawner(CurveEditorID);
	InTabManager->UnregisterTabSpawner(DetailsID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FAssetEditorToolkit | FSerializableObject interface

FName FluidAIUtilityPlannerEditorAgentEditor::GetToolkitFName() const
{
	return FName(AssetName.ToString());
}

FText FluidAIUtilityPlannerEditorAgentEditor::GetBaseToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidAIUtilityPlannerEditorAgentEditor::GetToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidAIUtilityPlannerEditorAgentEditor::GetToolkitToolTipText() const
{
	return FText::FromString(AssetName.ToString());
}

FLinearColor FluidAIUtilityPlannerEditorAgentEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FluidAIUtilityPlannerEditorAgentEditor::GetWorldCentricTabPrefix() const
{
	return AssetName.ToString();
}

FString FluidAIUtilityPlannerEditorAgentEditor::GetDocumentationLink() const
{
	return AssetName.ToString();
}

void FluidAIUtilityPlannerEditorAgentEditor::SaveAsset_Execute()
{
	CurveEditor->SetCurveOwner(nullptr);
	EdGraph->SaveToAsset();

	//Save Asset
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Asset->GetPackage());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, false);

	
	//Force Selection To ReSelect As Save Unselects
	OnSelectedNodesChanged(TSet<UObject*>());
}

void FluidAIUtilityPlannerEditorAgentEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graph

void FluidAIUtilityPlannerEditorAgentEditor::Initialize(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UFluidAIUtilityPlannerAgent* AssetIn)
{
	//Get Asset Name
	Asset = AssetIn;
	AssetName = Asset->GetFName();
	
	InitializeGraphEditor();
	InitializeCurveEditor();
	InitializeDetailsView();
	InitializeToolbar();

	// Layout
	FName LayoutID = Asset->GetClass()->GetFName();
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutID);
	const TSharedRef <FTabManager::FArea> Area = FTabManager::NewPrimaryArea();
	
	//AI Utility Planner Graph Editor
	Area->Split(FTabManager::NewStack()
				->AddTab(GraphEditorID, ETabState::OpenedTab));

	//Curve Editor
	Area->Split(FTabManager::NewStack()
				->AddTab(CurveEditorID, ETabState::OpenedTab));
	
	//Details View
	Area->Split(FTabManager::NewStack()
		->AddTab(DetailsID, ETabState::SidebarTab,ESidebarLocation::Right,10.0f));
	Layout->AddArea(Area);

	//Initialize Asset Editor
	FAssetEditorToolkit::InitAssetEditor(Mode,InitToolkitHost,"FluidAIUtilityPlanner",Layout,false,true,Asset);

	//Load EdGraph From Asset
	EdGraph->LoadFromAsset();

	//Set Asset As Details View Object
	DetailsView->SetObject(Asset);
}

void FluidAIUtilityPlannerEditorAgentEditor::InitializeGraphEditor()
{
	//Create Graph Editor Commands
	{
		FGenericCommands::Register();
		FGraphEditorCommands::Register();
		
		GraphEditorCommands = MakeShareable(new FUICommandList);
	
		//Delete Action
		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateRaw(this, &FluidAIUtilityPlannerEditorAgentEditor::OnCommandDeleteNodes),
			FCanExecuteAction::CreateRaw(this,  &FluidAIUtilityPlannerEditorAgentEditor::CanDoCommand));
	}
	
	//Create Appearance Info | In Events
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString("Fluid AI Utility Planner Graph");
	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FluidAIUtilityPlannerEditorAgentEditor::OnSelectedNodesChanged);
	
	//Create EdGraph
	EdGraph = CastChecked<UFluidAIUtilityPlannerEditorAgentEdGraph>(FBlueprintEditorUtils::CreateNewGraph(Asset, NAME_None, UFluidAIUtilityPlannerEditorAgentEdGraph::StaticClass(), UFluidAIUtilityPlannerEditorAgentEdGraphSchema::StaticClass()));
	EdGraph->Asset = Asset;
	
	//Create Graph Editor
	GraphEditor = SNew(SGraphEditor)
	.AdditionalCommands(GraphEditorCommands)
	.IsEditable(true)
	.Appearance(AppearanceInfo)
	.GraphToEdit(EdGraph)
	.GraphEvents(InEvents)
	.AutoExpandActionMenu(true)
	.ShowGraphStateOverlay(false);
}

void FluidAIUtilityPlannerEditorAgentEditor::InitializeCurveEditor()
{
	//Create Graph Editor
	CurveEditor = SNew(SCurveEditor)
	.ViewMinInput(0.0f)
	.ViewMaxInput(1.0f)
	.HideUI(false)
	.DrawCurve(true)
	.ViewMinOutput(0.0f)
	.ViewMaxOutput(1.0f)
	.TimelineLength(0)
	.AllowZoomOutput(false)
	.ShowInputGridNumbers(true)
	.ShowOutputGridNumbers(true)
	.ShowZoomButtons(false)
	.ZoomToFitHorizontal(false)
	.XAxisName(FString("Input"))
	.YAxisName(FString("Output"))
	.ShowCurveSelector(true);

	//Force Nullptr
	CurveEditor->SetCurveOwner(nullptr);
}

void FluidAIUtilityPlannerEditorAgentEditor::InitializeDetailsView()
{
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;
	Args.bAllowSearch = false;
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	DetailsView = PropertyModule.CreateDetailView(Args);
	DetailsView->OnFinishedChangingProperties().AddRaw(this, &FluidAIUtilityPlannerEditorAgentEditor::OnFinishedChangingProperties);
}

void FluidAIUtilityPlannerEditorAgentEditor::InitializeToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FluidAIUtilityPlannerEditorAgentEditor::FillToolbar));
	AddToolbarExtender(ToolbarExtender);
}

void FluidAIUtilityPlannerEditorAgentEditor::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Spawn Tabs

TSharedRef<SDockTab> FluidAIUtilityPlannerEditorAgentEditor::SpawnTabGraphEditor(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(FText::FromString("Fluid AI Utility Planner Graph"))
	[
		GraphEditor.ToSharedRef()
	];
}

TSharedRef<SDockTab> FluidAIUtilityPlannerEditorAgentEditor::SpawnTabCurveEditor(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(FText::FromString("Curves"))
	[
		CurveEditor.ToSharedRef()
	];
}

TSharedRef<SDockTab> FluidAIUtilityPlannerEditorAgentEditor::SpawnTabDetails(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	//	.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.TabColorScale(GetTabColorScale())
		[
			DetailsView.ToSharedRef()
		];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Input

void FluidAIUtilityPlannerEditorAgentEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	CurveEditor->SetCurveOwner(nullptr);
	EdGraph->ApplyUtilityModelNode();
	EdGraph->NotifyGraphChanged();
}

void FluidAIUtilityPlannerEditorAgentEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	//EdGraph->ApplyUtilityModelNode();
	
	//No Node Selected So Return
	DetailsView->SetObject(nullptr);
	CurveEditor->SetCurveOwner(nullptr);
	if (!GraphEditor->GetSingleSelectedNode())
	{
		//Set Asset As Details View Object
		DetailsView->SetObject(Asset);
		
		return;
	}
	
	try
	{
		//Set Curve Editor Object
		if (GraphEditor->GetSingleSelectedNode()->IsA(UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel::StaticClass()))
		{
			auto SelectedUtilityModel = ((UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel*)GraphEditor->GetSingleSelectedNode());
			if (SelectedUtilityModel != nullptr && SelectedUtilityModel->EditorCurve != nullptr)
				CurveEditor->SetCurveOwner(((UFluidAIUtilityPlannerEditorAgentEdGraphNodeAgentUtilitySubModel*)GraphEditor->GetSingleSelectedNode())->EditorCurve);
		}
		
		//Set Details View Object
		DetailsView->SetObject(GraphEditor->GetSingleSelectedNode());
	}
	catch (const std::overflow_error& e)
	{
		auto whatToAvoidError = e.what();
		//Set Asset As Details View Object
		DetailsView->SetObject(Asset);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graph Editor Commands

bool FluidAIUtilityPlannerEditorAgentEditor::CanDoCommand()
{
	return true;
}

void FluidAIUtilityPlannerEditorAgentEditor::OnCommandDeleteNodes()
{
	EdGraph->OnCommandDeleteNodes(GraphEditor);
}