#include "FluidNarratorLocalizationEditor.h"

//Loggig
DEFINE_LOG_CATEGORY(LogFluidNarratorLocalizationEditor);

/*=============================================================================
Fluid Narrator Localization Editor : Constant Tab Editor IDS
=============================================================================*/ 

const FName FluidNarratorEditorLocalizationTextsTabID(TEXT("FluidNarratorLocalizationTexts"));
const FName FluidNarratorEditorLocalizationCulturesTabID(TEXT("FluidNarratorLocalizationCultures"));
const FName FluidNarratorEditorLocalizationMiscAssetDetailsTabID(TEXT("FluidNarratorLocalizationMiscAssetDetails"));

/*=============================================================================
Fluid Narrator Localization Editor : Constructors
=============================================================================*/

FluidNarratorLocalizationEditor::FluidNarratorLocalizationEditor()
{
}

FluidNarratorLocalizationEditor::~FluidNarratorLocalizationEditor()
{
	//UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

/*=============================================================================
Fluid Narrator Localization Editor : FAssetEditorToolkit | FSerializableObject interface
=============================================================================*/

FName FluidNarratorLocalizationEditor::GetToolkitFName() const
{
	return FName(AssetName.ToString());
}

FText FluidNarratorLocalizationEditor::GetBaseToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidNarratorLocalizationEditor::GetToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidNarratorLocalizationEditor::GetToolkitToolTipText() const
{
	return FText::FromString(AssetName.ToString());
}

FLinearColor FluidNarratorLocalizationEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FluidNarratorLocalizationEditor::GetWorldCentricTabPrefix() const
{
	return AssetName.ToString();
}

FString FluidNarratorLocalizationEditor::GetDocumentationLink() const
{
	return AssetName.ToString();
}

void FluidNarratorLocalizationEditor::SaveAsset_Execute()
{
	//Save Cultures And Texts
	CulturesWidget->Save();
	TextsWidget->HandelSave();
	
	//Save Asset
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Asset->GetPackage());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
}

void FluidNarratorLocalizationEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
}

/*=============================================================================
Fluid Narrator Localization Editor : Register Tab Spawners
=============================================================================*/

void FluidNarratorLocalizationEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FluidNarratorEditorLocalizationTextsTabID, FOnSpawnTab::CreateSP(this, &FluidNarratorLocalizationEditor::SpawnTabTexts))
	.SetDisplayName(FText::FromString("Texts"));
	
	InTabManager->RegisterTabSpawner(FluidNarratorEditorLocalizationCulturesTabID, FOnSpawnTab::CreateSP(this, &FluidNarratorLocalizationEditor::SpawnTabCultures))
	.SetDisplayName(FText::FromString("Cultures"));
	
	InTabManager->RegisterTabSpawner(FluidNarratorEditorLocalizationMiscAssetDetailsTabID, FOnSpawnTab::CreateSP(this, &FluidNarratorLocalizationEditor::SpawnTabMiscAssetDetails))
	.SetDisplayName(FText::FromString("Developer Settings"));
}

void FluidNarratorLocalizationEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(FluidNarratorEditorLocalizationTextsTabID);
	InTabManager->UnregisterTabSpawner(FluidNarratorEditorLocalizationCulturesTabID);
	InTabManager->UnregisterTabSpawner(FluidNarratorEditorLocalizationMiscAssetDetailsTabID);
}

/*=============================================================================
Fluid Narrator Localization Editor : Initialize
=============================================================================*/

void FluidNarratorLocalizationEditor::Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidNarratorLocalization* AssetIn)
{
	//Get Marshaller Syntax Developer Settings
	NarratorDeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();

	//Set Asset | Asset Name
	Asset = AssetIn;
	AssetName = AssetIn->GetFName();
	
	//Toolbar
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FluidNarratorLocalizationEditor::InitializeToolbar));
		AddToolbarExtender(ToolbarExtender);
	}

	//Cultures Widget
	{
		CulturesWidget = SNew(SFluidNarratorLocalizationCulturesSlateWidget);
		CulturesWidget->LocalizationAsset = AssetIn;
		CulturesWidget->Editor = this;
		CulturesWidget->HandelLoad();
	}
	
	//Texts View
	{
		TextsWidget = SNew(SFluidNarratorLocalizationTextsSlateWidget);
		TextsWidget->LocalizationAsset = AssetIn;
		TextsWidget->Editor = this;
		TextsWidget->HandelLoad();
	}
	
	//Asset Misc Settings Details View
	{
		FDetailsViewArgs Args;
		Args.bHideSelectionTip = true;
		Args.NotifyHook = this;
		Args.bAllowSearch = false;
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		AssetMiscSettingsDetailsView = PropertyModule.CreateDetailView(Args);
		AssetMiscSettingsDetailsView->OnFinishedChangingProperties().AddRaw(this, &FluidNarratorLocalizationEditor::HandelAssetPropertyChanged);
		AssetMiscSettingsDetailsView->SetObject(Asset);
	}
	
	// Layout | Area
	FName LayoutID = "FluidNarratorLocalizationEditor";
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutID);
	const TSharedRef <FTabManager::FArea> Area = FTabManager::NewPrimaryArea();

	//Add Tabs
	Area->Split(FTabManager::NewStack()
	->AddTab(FluidNarratorEditorLocalizationTextsTabID, ETabState::OpenedTab)
	->AddTab(FluidNarratorEditorLocalizationCulturesTabID, ETabState::OpenedTab)
	->AddTab(FluidNarratorEditorLocalizationMiscAssetDetailsTabID, ETabState::SidebarTab, ESidebarLocation::Right,40.0f));
	
	//Add Area To Layout
	Layout->AddArea(Area);
	
	//Initialize Asset Editor
	FAssetEditorToolkit::InitAssetEditor(Mode,InitToolkitHost,"FluidNarratorLocalizationEditor",Layout,false,true,AssetIn);
}

void FluidNarratorLocalizationEditor::InitializeToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection("FluidNarratorCultures");
	{
	};
}

/*=============================================================================
Fluid Narrator Localization Editor : Spawn Tabs
=============================================================================*/

TSharedRef<SDockTab> FluidNarratorLocalizationEditor::SpawnTabTexts(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).Label(FText::FromString("Texts"));
	SpawnedTab->SetContent(TextsWidget.ToSharedRef());
	return SpawnedTab;
}

TSharedRef<SDockTab> FluidNarratorLocalizationEditor::SpawnTabCultures(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).Label(FText::FromString("Cultures"));
	SpawnedTab->SetContent(CulturesWidget.ToSharedRef());
	return SpawnedTab;
}

TSharedRef<SDockTab> FluidNarratorLocalizationEditor::SpawnTabMiscAssetDetails(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).Label(FText::FromString("Misc"));
	SpawnedTab->SetContent(AssetMiscSettingsDetailsView.ToSharedRef());
	return SpawnedTab;
}

/*=============================================================================
Fluid Narrator Localization Editor : Handles
=============================================================================*/

void FluidNarratorLocalizationEditor::HandelAssetPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	//Mark Dirty (Needs To Save)
	Asset->MarkPackageDirty();
}