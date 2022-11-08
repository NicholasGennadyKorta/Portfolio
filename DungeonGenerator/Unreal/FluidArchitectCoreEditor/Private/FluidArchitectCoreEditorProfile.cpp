// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidArchitectCoreEditorProfile.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Property Identifiers


struct FFluidArchitectCoreEditorProfileTabs
{
	static const FName DetailsID;
	static const FName GraphEditorID;
};

const FName FFluidArchitectCoreEditorProfileTabs::DetailsID(TEXT("FluidArchitectCoreDetails"));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Constructors

FluidArchitectCoreEditorProfile::FluidArchitectCoreEditorProfile()
{
}

FluidArchitectCoreEditorProfile::~FluidArchitectCoreEditorProfile()
{
	//UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Toolkit interface

void FluidArchitectCoreEditorProfile::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	//Graph Editors
	CurrentGraphEditorToInitializeByIndex = 0;
	for (int i = 0; i < Asset->Modules.Num(); i++)
	{
		if (Asset->Modules[i].ViewportType=="graphEditor")
		{
			InTabManager->RegisterTabSpawner(Asset->Modules[i].UID, FOnSpawnTab::CreateSP(this, &FluidArchitectCoreEditorProfile::SpawnTabGraphEditor))
			.SetDisplayName(FText::FromString(Asset->Modules[i].ViewportName.ToString()))
			.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), ""));
		}
	}

	//Previews Viewports
	CurrentPreviewViewportsToInitializeByIndex = 0;
	for (int i = 0; i < Asset->Modules.Num(); i++)
	{
		if (Asset->Modules[i].ViewportType=="previewViewport")
		{
			InTabManager->RegisterTabSpawner(Asset->Modules[i].UID, FOnSpawnTab::CreateSP(this, &FluidArchitectCoreEditorProfile::SpawnTabPreviewViewports))
			.SetDisplayName(FText::FromString(Asset->Modules[i].ViewportName.ToString()))
			.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), ""));
		}
	}
	
	//Details
	InTabManager->RegisterTabSpawner(FFluidArchitectCoreEditorProfileTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FluidArchitectCoreEditorProfile::SpawnTabDetails))
		.SetDisplayName(FText::FromString("Details"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GenericEditor.Tabs.Properties"));
}

void FluidArchitectCoreEditorProfile::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	InTabManager->UnregisterTabSpawner(FFluidArchitectCoreEditorProfileTabs::DetailsID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FAssetEditorToolkit | FSerializableObject interface

FName FluidArchitectCoreEditorProfile::GetToolkitFName() const
{
	return FName(AssetName.ToString());
}

FText FluidArchitectCoreEditorProfile::GetBaseToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidArchitectCoreEditorProfile::GetToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidArchitectCoreEditorProfile::GetToolkitToolTipText() const
{
	return FText::FromString(AssetName.ToString());
}

FLinearColor FluidArchitectCoreEditorProfile::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FluidArchitectCoreEditorProfile::GetWorldCentricTabPrefix() const
{
	return AssetName.ToString();
}

FString FluidArchitectCoreEditorProfile::GetDocumentationLink() const
{
	return AssetName.ToString();
}

void FluidArchitectCoreEditorProfile::SaveAsset_Execute()
{	
	//Save EdGraphs
	for (int i = 0; i < EdGraphs.Num(); i++)
		EdGraphs[i]->SaveToAsset(Asset);
}

void FluidArchitectCoreEditorProfile::AddReferencedObjects(FReferenceCollector& Collector)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graph

void FluidArchitectCoreEditorProfile::Initialize(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UFluidArchitectCoreProfile* AssetIn)
{
	//Get Asset Name
	Asset = AssetIn;
	AssetName = Asset->GetFName();
	Asset->Initialize();
	
	InitializeGraphEditors();
	InitializePreviewViewports();
	InitializeDetailsView();
	InitializeToolbar();

	// Layout
	FName LayoutID = Asset->GetClass()->GetFName();
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutID);
	const TSharedRef <FTabManager::FArea> Area = FTabManager::NewPrimaryArea();

	//Modules
	for (int i = 0; i < Asset->Modules.Num(); i++)
		if (Asset->Modules[i].ViewportType=="graphEditor" || Asset->Modules[i].ViewportType=="previewViewport")
		{
			Area->Split(FTabManager::NewStack()
				->AddTab(Asset->Modules[i].UID, ETabState::OpenedTab));
		}

	//Details View
	Area->Split(FTabManager::NewStack()
		->AddTab(FFluidArchitectCoreEditorProfileTabs::DetailsID, ETabState::SidebarTab,ESidebarLocation::Right,10.0f));

	//Add Area
	Layout->AddArea(Area);

	//Initialize Asset Editor
	FAssetEditorToolkit::InitAssetEditor(Mode,InitToolkitHost,"FluidArchitect",Layout,false,true,Asset);
	
	//Set Details View Object To Asset
	DetailsView->SetObject(Asset);
}

void FluidArchitectCoreEditorProfile::InitializeGraphEditors()
{
	//Create Graph Editor Commands
	{
		FGenericCommands::Register();
		FGraphEditorCommands::Register();
		
		GraphEditorCommands = MakeShareable(new FUICommandList);
	
		//Delete Action
		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateRaw(this, &FluidArchitectCoreEditorProfile::OnCommandDeleteNodes),
			FCanExecuteAction::CreateRaw(this,  &FluidArchitectCoreEditorProfile::CanDoCommand));

		//Copy Action
		GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
			FExecuteAction::CreateRaw(this, &FluidArchitectCoreEditorProfile::OnCommandCopyNodes),
			FCanExecuteAction::CreateRaw(this,  &FluidArchitectCoreEditorProfile::CanDoCommand));

		//Cut Action
		GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
			FExecuteAction::CreateRaw(this, &FluidArchitectCoreEditorProfile::OnCommandCutNodes),
			FCanExecuteAction::CreateRaw(this,  &FluidArchitectCoreEditorProfile::CanDoCommand));

		//Paste Action
		GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
			FExecuteAction::CreateRaw(this, &FluidArchitectCoreEditorProfile::OnCommandPasteNodes),
			FCanExecuteAction::CreateRaw(this,  &FluidArchitectCoreEditorProfile::CanDoCommand));

		//Duplicate Action
		GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateRaw(this, &FluidArchitectCoreEditorProfile::OnCommandDuplicateNodes),
			FCanExecuteAction::CreateRaw(this,  &FluidArchitectCoreEditorProfile::CanDoCommand));
	}

	GraphEditors.Empty();
	EdGraphs.Empty();
	for (int i = 0; i < Asset->Modules.Num(); i++)
	{
		if (Asset->Modules[i].ViewportType=="graphEditor")
		{
			//Create Appearance Info | In Events
			FGraphAppearanceInfo AppearanceInfo;
			AppearanceInfo.CornerText = FText::FromString(Asset->Modules[i].ViewportName.ToString());
			SGraphEditor::FGraphEditorEvents InEvents;
			InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FluidArchitectCoreEditorProfile::OnSelectedNodesChanged);

			//Create EdGraph
			auto const EdGraph = CastChecked<UFluidArchitectCoreEditorEdGraph>(FBlueprintEditorUtils::CreateNewGraph(Asset, NAME_None, UFluidArchitectCoreEditorEdGraph::StaticClass(), UFluidArchitectCoreEditorEdGraphSchema::StaticClass()));
			EdGraph->ModuleUID = Asset->Modules[i].UID;
			EdGraph->ModuleName = Asset->Modules[i].ViewportName;
			((UFluidArchitectCoreEditorEdGraphSchema*)EdGraph->GetSchema())->Asset = Asset;
			
			//Create Graph Editor
			auto NewGraphEditor = SNew(SGraphEditor)
			.AdditionalCommands(GraphEditorCommands)
			.IsEditable(true)
			.Appearance(AppearanceInfo)
			.GraphToEdit(EdGraph)
			.GraphEvents(InEvents)
			.AutoExpandActionMenu(true)
			.ShowGraphStateOverlay(false);

			//Add To Graph Arrays And Load Asset
			GraphEditors.Add(NewGraphEditor);
			EdGraphs.Add(EdGraph);
			EdGraph->LoadFromAsset(Asset);

			
		}
	}
}

void FluidArchitectCoreEditorProfile::InitializePreviewViewports()
{
	for (int i = 0; i < Asset->Modules.Num(); i++)
		if (Asset->Modules[i].ViewportType=="previewViewport")
		{
			auto ViewportWidget = SNew(SFluidArchitectCorePreviewViewportWidget);
			ViewportWidget->Asset = Asset;
			ViewportWidget->ModuleID = Asset->Modules[i].UID;
			ViewportWidget->ModuleName = Asset->Modules[i].ViewportName;
			PreviewViewports.Add(ViewportWidget);
		}
}

void FluidArchitectCoreEditorProfile::InitializeDetailsView()
{
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;
	Args.bAllowSearch = false;
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	DetailsView = PropertyModule.CreateDetailView(Args);
	DetailsView->OnFinishedChangingProperties().AddRaw(this, &FluidArchitectCoreEditorProfile::OnFinishedChangingProperties);
}

void FluidArchitectCoreEditorProfile::InitializeToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FluidArchitectCoreEditorProfile::FillToolbar));
	AddToolbarExtender(ToolbarExtender);
}

void FluidArchitectCoreEditorProfile::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	//Generate
	ToolbarBuilder.BeginSection("ExtendToolbarItemGenerate");
	{
		const FSlateIcon NewNodeIcon = FSlateIcon(TEXT("EditorStyle"), "GenericPlay");

		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FluidArchitectCoreEditorProfile::OnGenerate),
				FIsActionChecked()
			),
			NAME_None,
			FText::FromString("Generate"),
			FText::FromString("Generate"),
			NewNodeIcon
		);
	}
	ToolbarBuilder.EndSection();

	//Nice Separator :)
	ToolbarBuilder.BeginSection("ExtendToolbarItemModes");
	{
	}
	ToolbarBuilder.EndSection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Spawn Tabs

TSharedRef<SDockTab> FluidArchitectCoreEditorProfile::SpawnTabGraphEditor(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(FText::FromString(EdGraphs[CurrentGraphEditorToInitializeByIndex]->ModuleName.ToString()));

	if (GraphEditors[CurrentGraphEditorToInitializeByIndex].IsValid())
		SpawnedTab->SetContent(GraphEditors[CurrentGraphEditorToInitializeByIndex].ToSharedRef());

	CurrentGraphEditorToInitializeByIndex++;
	return SpawnedTab;
}

TSharedRef<SDockTab> FluidArchitectCoreEditorProfile::SpawnTabPreviewViewports(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(FText::FromString(PreviewViewports[CurrentPreviewViewportsToInitializeByIndex]->ModuleName.ToString()));
	
	SpawnedTab->SetContent(PreviewViewports[CurrentPreviewViewportsToInitializeByIndex].ToSharedRef());

	CurrentPreviewViewportsToInitializeByIndex++;
	return SpawnedTab;
}

TSharedRef<SDockTab> FluidArchitectCoreEditorProfile::SpawnTabDetails(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.TabColorScale(GetTabColorScale())
		[
			// Provide the details view as this tab its content
			DetailsView.ToSharedRef()
		];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input

void FluidArchitectCoreEditorProfile::OnGenerate()
{
	SaveAsset_Execute();
	Asset->Generate();
}

void FluidArchitectCoreEditorProfile::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	// Let EdGraphs Know There Was A Change
	for (int i = 0; i < EdGraphs.Num(); i++)
		EdGraphs[i]->NotifyGraphChanged();
}

void FluidArchitectCoreEditorProfile::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	//Get Selections
	TArray<UObject*> Selection;
	for (UObject* SelectionEntry : NewSelection)
		Selection.Add(SelectionEntry);

	//Set Details View Object To Asset
	if (Selection.Num() == 0)
	{
		DetailsView->SetObject(Asset);
		return;
	}
	
	//Set Details View Object To Selected Node
	if ((UFluidArchitectCoreEditorEdGraphNode*) Selection[0])
	{
		DetailsView->SetObject(((UFluidArchitectCoreEditorEdGraphNode*) Selection[0])->GraphNodeEvent);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graph Editor Commands

bool FluidArchitectCoreEditorProfile::CanDoCommand()
{
	return true;
}

void FluidArchitectCoreEditorProfile::OnCommandDeleteNodes()
{
	for (int i = 0; i < EdGraphs.Num(); i++)
			EdGraphs[i]->OnCommandDeleteNodes(GraphEditors[i]);
}

void FluidArchitectCoreEditorProfile::OnCommandCopyNodes()
{
	for (int i = 0; i < EdGraphs.Num(); i++)
		EdGraphs[i]->OnCommandCopyNodes(GraphEditors[i]);
}

void FluidArchitectCoreEditorProfile::OnCommandCutNodes()
{
	for (int i = 0; i < EdGraphs.Num(); i++)
		EdGraphs[i]->OnCommandCutNodes(GraphEditors[i]);
}

void FluidArchitectCoreEditorProfile::OnCommandPasteNodes()
{
	for (int i = 0; i < EdGraphs.Num(); i++)
		EdGraphs[i]->OnCommandPasteNodes(GraphEditors[i]);
}

void FluidArchitectCoreEditorProfile::OnCommandDuplicateNodes()
{
	for (int i = 0; i < EdGraphs.Num(); i++)
		EdGraphs[i]->OnCommandDuplicateNodes(GraphEditors[i]);
}
