#include "FluidNarratorScreenplayEditor.h"

/*=============================================================================
Fluid Narrator Localization Editor : Logging | Constant Tab Editor IDS
=============================================================================*/

DEFINE_LOG_CATEGORY(LogFluidNarratorScreenplayEditor);

const FName ScreenplayTextEditorTabID(TEXT("FluidNarratorScreenplayTextEditor"));
const FName ScreenplaySimulatorDataTabID(TEXT("FluidNarratorScreenplaySimulatorData"));
const FName ScreenplaySimulatorTabID(TEXT("FluidNarratorScreenplaySimulator"));
const FName ScreenplayDeveloperSettingsDetailsTabID(TEXT("FluidNarratorScreenplayDeveloperSettingsDetails"));

/*=============================================================================
Fluid Narrator Screenplay Editor : Constructors
=============================================================================*/

FluidNarratorScreenplayEditor::FluidNarratorScreenplayEditor()
{
}

FluidNarratorScreenplayEditor::~FluidNarratorScreenplayEditor()
{
	//UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

/*=============================================================================
Fluid Narrator Screenplay Editor : FAssetEditorToolkit | FSerializableObject interface
=============================================================================*/

FName FluidNarratorScreenplayEditor::GetToolkitFName() const
{
	return FName(AssetName.ToString());
}

FText FluidNarratorScreenplayEditor::GetBaseToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidNarratorScreenplayEditor::GetToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidNarratorScreenplayEditor::GetToolkitToolTipText() const
{
	return FText::FromString(AssetName.ToString());
}

FLinearColor FluidNarratorScreenplayEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FluidNarratorScreenplayEditor::GetWorldCentricTabPrefix() const
{
	return AssetName.ToString();
}

FString FluidNarratorScreenplayEditor::GetDocumentationLink() const
{
	return AssetName.ToString();
}

void FluidNarratorScreenplayEditor::SaveAsset_Execute()
{
	//Save Asset
	Asset->Text = ScriptViewWidget->GetText().ToString();
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Asset->GetPackage());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, false);
}

void FluidNarratorScreenplayEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
}

/*=============================================================================
Fluid Narrator Screenplay Editor : Register Tab Spawners
=============================================================================*/

void FluidNarratorScreenplayEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	InTabManager->RegisterTabSpawner(ScreenplayTextEditorTabID, FOnSpawnTab::CreateSP(this, &FluidNarratorScreenplayEditor::SpawnTabTextEditor))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), ""));

	InTabManager->RegisterTabSpawner(ScreenplaySimulatorDataTabID, FOnSpawnTab::CreateSP(this, &FluidNarratorScreenplayEditor::SpawnTabSimulatorData))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), ""));

	InTabManager->RegisterTabSpawner(ScreenplaySimulatorTabID, FOnSpawnTab::CreateSP(this, &FluidNarratorScreenplayEditor::SpawnTabSimulator))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), ""));
	
	InTabManager->RegisterTabSpawner( ScreenplayDeveloperSettingsDetailsTabID, FOnSpawnTab::CreateSP(this, &FluidNarratorScreenplayEditor::SpawnTabEditorSettingsDetails))
		.SetDisplayName(FText::FromString("Details"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GenericEditor.Tabs.Properties"));
}

void FluidNarratorScreenplayEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(ScreenplayTextEditorTabID);
	InTabManager->UnregisterTabSpawner(ScreenplaySimulatorDataTabID);
	InTabManager->UnregisterTabSpawner(ScreenplaySimulatorTabID);
	InTabManager->UnregisterTabSpawner( ScreenplayDeveloperSettingsDetailsTabID);
}

/*=============================================================================
Fluid Narrator Screenplay Editor : Initialize
=============================================================================*/

void FluidNarratorScreenplayEditor::Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidNarratorScreenplay* AssetIn)
{
	Asset = AssetIn;
	AssetName = Asset->GetFName();

	//Get Marshaller Syntax Developer Settings
	NarratorDeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	TSharedRef<FluidNarratorScreenplayMarshaller> MarshallerRef = FluidNarratorScreenplayMarshaller::Create();
	Marshaller = &MarshallerRef.Get();

	//Toolbar
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FluidNarratorScreenplayEditor::InitializeToolbar));
		AddToolbarExtender(ToolbarExtender);
	}
	
	//Create Text Editor Text Block
	{
		ScriptViewWidget = SNew(SFluidNarratorScreenplayScriptViewWidget);
		ScriptViewWidget->Editor = this;
		ScriptViewWidget->ScreenplayController = &ScreenplaySystem;
		ScriptViewWidget->SetText(FText::FromString(Asset->Text));
	}

	//Create Text Editor Text Line Numbers
	{
		TextEditorTextBoxLineNumbers = SNew(SMultiLineEditableTextBox)
		.IsReadOnly(true)
		.BackgroundColor(NarratorDeveloperSettings->BackgroundReadOnlyColorTint)
		.Marshaller(MarshallerRef);
	}

	//Create Variables Text Block
	{
		SimulatorVariablesTextBlock = SNew(SMultiLineEditableTextBox)
		.BackgroundColor(NarratorDeveloperSettings->BackgroundColorTint);
	}

	//Simulator Widget
	{
		SimulatorWidget = SNew(SFluidNarratorScreenplaySimulatorSlateWidget);
		SimulatorWidget->ScreenplaySystem = &ScreenplaySystem;
		SimulatorWidget->Editor = this;
	}

	//Editor Settings Detail View
	{
		FDetailsViewArgs Args;
		Args.bHideSelectionTip = true;
		Args.NotifyHook = this;
		Args.bAllowSearch = false;
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		EditorSettingsDetailView = PropertyModule.CreateDetailView(Args);
		EditorSettingsDetailView->OnFinishedChangingProperties().AddRaw(this, &FluidNarratorScreenplayEditor::HandelAssetPropertyChanged);
		EditorSettingsDetailView->SetObject(NarratorDeveloperSettings);
	}
	
	// Layout | Area
	FName LayoutID = Asset->GetClass()->GetFName();
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutID);
	const TSharedRef <FTabManager::FArea> Area = FTabManager::NewPrimaryArea();

	//Add Tabs
	Area->Split(FTabManager::NewStack()
		->AddTab(ScreenplayTextEditorTabID, ETabState::OpenedTab, ESidebarLocation::None,0.0f)
		->AddTab(ScreenplaySimulatorDataTabID, ETabState::SidebarTab, ESidebarLocation::Right,60.0f)
		->AddTab(ScreenplaySimulatorTabID, ETabState::SidebarTab, ESidebarLocation::Right,60.0f)
		->AddTab(ScreenplayDeveloperSettingsDetailsTabID, ETabState::SidebarTab, ESidebarLocation::Right,40.0f));
	
	//Add Area To Layout
	Layout->AddArea(Area);
	
	//Initialize Asset Editor
	FAssetEditorToolkit::InitAssetEditor(Mode,InitToolkitHost,"FluidNarrator",Layout,false,true,Asset);
	
	//Start With No Screenplay Task
	ScreenplayTask = -1;
}

void FluidNarratorScreenplayEditor::InitializeToolbar(FToolBarBuilder& ToolbarBuilder)
{
	
	//Generate
	ToolbarBuilder.BeginSection("ExtendToolbarItemGenerate");
	{
		//Simulate Button
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FluidNarratorScreenplayEditor::HandelToolbarSimulatorButtonPressed),
				FIsActionChecked()
			),
			NAME_None,
			FText::FromString("Simulator"),
			FText::FromString("Run Simulator"),
			FSlateIcon(TEXT("EditorStyle"), "GenericPlay")
		);
		
		//Culture Editable Text Box
		{
			//Create Widget
			SAssignNew(ToolbarSimulatorCultureEditableTextBox,SEditableTextBox)
			.Text(FText::FromString("Simulator Culture"))
			.MinDesiredWidth(125);

			//Add Widget
			ToolbarBuilder.AddWidget(ToolbarSimulatorCultureEditableTextBox.ToSharedRef());
		}

		//Culture Localization Property View
		{
			//Create Widget
			//SAssignNew(ToolbarSimulatorLocalizationAssetPropertyView,SSingleProperty)
			//.PropertyName(FName("Localization"))
			//.Object(Localization);
			
			//Add Widget
			//ToolbarBuilder.AddWidget(ToolbarSimulatorLocalizationAssetPropertyView.ToSharedRef());
		}

	
		//Separator
		ToolbarBuilder.AddSeparator();
	}
	ToolbarBuilder.EndSection();
}

/*=============================================================================
Fluid Narrator Screenplay Editor : Spawn Tabs
=============================================================================*/

TSharedRef<SDockTab> FluidNarratorScreenplayEditor::SpawnTabTextEditor(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(FText::FromString("Text Editor"))
	.TabColorScale(GetTabColorScale())
	[
		ScriptViewWidget.ToSharedRef()
	];
}

TSharedRef<SDockTab> FluidNarratorScreenplayEditor::SpawnTabSimulatorData(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(FText::FromString("Simulator Data"))
	.TabColorScale(GetTabColorScale())
	[
		SimulatorVariablesTextBlock.ToSharedRef()
	];
}

TSharedRef<SDockTab> FluidNarratorScreenplayEditor::SpawnTabSimulator(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(FText::FromString("Simulator"))
	.TabColorScale(GetTabColorScale())
	[
		SimulatorWidget.ToSharedRef()
	];
}

TSharedRef<SDockTab> FluidNarratorScreenplayEditor::SpawnTabEditorSettingsDetails(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(FText::FromString("Developer Settings"))
	.TabColorScale(GetTabColorScale())
	[
		EditorSettingsDetailView.ToSharedRef()
	];
}

/*=============================================================================
Fluid Narrator Screenplay Editor : Handles
=============================================================================*/

void FluidNarratorScreenplayEditor::HandelScreenplayStart()
{
	ScriptViewWidget->SetIsReadOnly(true);
	SimulatorVariablesTextBlock->SetIsReadOnly(true);
}

void FluidNarratorScreenplayEditor::HandelScreenplayStop()
{
	ScriptViewWidget->SetIsReadOnly(false);
	SimulatorVariablesTextBlock->SetIsReadOnly(false);
	ScreenplayTask = -1;

	//Read Variables
	FString VariablesText;
	FluidNarratorScreenplaySystem::ParseVariablesToString(ScreenplaySystem.Variables,VariablesText);
	SimulatorVariablesTextBlock->SetText(FText::FromString(VariablesText));
}

void FluidNarratorScreenplayEditor::HandelScreenplayTick()
{
	//Read Variables
	FString VariablesText;
	FluidNarratorScreenplaySystem::ParseVariablesToString(ScreenplaySystem.Variables,VariablesText);
	FString S = "Task Response Auto Time : " +FString::SanitizeFloat(ScreenplaySystem.TasksAutoResponseComponents[ScreenplayTask].TimerCurrent) + "\n" + VariablesText;
	SimulatorVariablesTextBlock->SetText(FText::FromString(S));
}

void FluidNarratorScreenplayEditor::HandelToolbarSimulatorButtonPressed()
{
	NarratorDeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	
	//Start
	if (ScreenplayTask == -1)
	{
		//Write Variables
		FluidNarratorScreenplaySystem::ParseVariablesFromString(SimulatorVariablesTextBlock->GetText().ToString(),ScreenplaySystem.Variables);

		//Run
		ScreenplaySystem.Localization = nullptr;
		ScreenplaySystem.StartBySectionAuto(FName(ToolbarSimulatorCultureEditableTextBox->GetText().ToString()),ScriptViewWidget->GetText().ToString(),nullptr,SimulatorWidget.Get(),ScreenplayTask);

		//Read Variables
		FString VariablesText;
		FluidNarratorScreenplaySystem::ParseVariablesToString(ScreenplaySystem.Variables,VariablesText);
		SimulatorVariablesTextBlock->SetText(FText::FromString(VariablesText));
	}

	//Stop
	else
	{
		ScreenplaySystem.Stop(ScreenplayTask,ScreenplayTask);
	}
}

void FluidNarratorScreenplayEditor::HandelAssetPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	//Save Developer Settings
	UFluidNarratorDeveloperSettings* DeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	DeveloperSettings->SaveConfig();

	//Update Script View Widget
	ScriptViewWidget->HandelAssetPropertyChanged(PropertyChangedEvent);
	
	//Update Variables
	Marshaller->MakeDirty();
	SimulatorVariablesTextBlock->SetTextBoxBackgroundColor(NarratorDeveloperSettings->BackgroundColorTint);
	SimulatorVariablesTextBlock->SetText(SimulatorVariablesTextBlock->GetText());
	SimulatorVariablesTextBlock->Refresh();
	Marshaller->MakeDirty();
}