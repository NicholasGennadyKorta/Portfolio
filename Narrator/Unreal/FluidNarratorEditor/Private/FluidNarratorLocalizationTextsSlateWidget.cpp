// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidNarratorLocalizationTextsSlateWidget.h"
#include "SlateOptMacros.h"

/*=============================================================================
Fluid Narrator Localization Texts Widget : Construct
=============================================================================*/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFluidNarratorLocalizationTextsSlateWidget::Construct(const FArguments& InArgs)
{
	//Get Fluid Narrator Developer Settings
	UFluidNarratorDeveloperSettings* DeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	
	//Layout
	ChildSlot
	[
		//Vertical Box
	    SNew(SVerticalBox)
	    +SVerticalBox::Slot()
		.AutoHeight()
		[
			//Toolbar
			SNew(SHorizontalBox)
		
			//Scan
			+SHorizontalBox::Slot()
			.Padding(FMargin(0,4))
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("StatusBar.StatusBarButton"))
				.OnPressed(this,&SFluidNarratorLocalizationTextsSlateWidget::HandelScan)
				[
					SNew(SHorizontalBox )
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(FLinearColor::Gray))
						.Font( FEditorStyle::Get().GetFontStyle( "FontAwesome.11" ) )
						.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
						.Text( FText::FromString(FString(TEXT("\xf002"))))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
						.Text(FText::FromString(" Scan "))
					]
				]
			]
			
			//Separator
			+SHorizontalBox::Slot()
			.MaxWidth(1)
			[
				SNew(SSeparator)
				.Thickness(1)
			]
			
			//Export Files
			+SHorizontalBox::Slot()
			.Padding(FMargin(2,4))
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("StatusBar.StatusBarButton"))
				.OnPressed(this,&SFluidNarratorLocalizationTextsSlateWidget::HandelFilesExport)
				[
					SNew( SHorizontalBox )
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(FLinearColor::Gray))
						.Font( FEditorStyle::Get().GetFontStyle( "FontAwesome.11" ) )
						.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
						.Text( FText::FromString(FString(TEXT("\xf093"))))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
						.Text(FText::FromString(" Export"))
					]
				]
			]

			//Separator
			+SHorizontalBox::Slot()
			.MaxWidth(1)
			[
				SNew(SSeparator)
				.Thickness(1)
			]

			//Import Files
			+SHorizontalBox::Slot()
			.Padding(FMargin(2,4))
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("StatusBar.StatusBarButton"))
				.OnPressed(this,&SFluidNarratorLocalizationTextsSlateWidget::HandelFilesImport)
				[
					SNew( SHorizontalBox )
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(FLinearColor::Gray))
						.Font( FEditorStyle::Get().GetFontStyle( "FontAwesome.11" ) )
						.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
						.Text( FText::FromString(FString(TEXT("\xf019"))))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
						.Text(FText::FromString(" Import"))
					]
				]
			]

			//Separator
			+SHorizontalBox::Slot()
			.MaxWidth(1)
			[
				SNew(SSeparator)
				.Thickness(1)
			]

			//Translate
			+ SHorizontalBox::Slot()
			.Padding(FMargin(2,4))
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("StatusBar.StatusBarButton"))
				.OnPressed(this,&SFluidNarratorLocalizationTextsSlateWidget::HandelTranslateText)
				[
					SNew( SHorizontalBox )
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(FLinearColor::Gray))
						.Font( FEditorStyle::Get().GetFontStyle( "FontAwesome.11" ) )
						.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
						.Text( FText::FromString(FString(TEXT("\xf0ac"))))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
						.Text(FText::FromString(" Translate"))
					]
				]
			]
			
			//Separator
			+SHorizontalBox::Slot()
			.MaxWidth(3)
			[
				SNew(SSeparator)
				.Thickness(3)
			]
			
			//Namespace ComboBox
			+ SHorizontalBox::Slot()
			.Padding(FMargin(4,4))
			.MaxWidth(135)
			[
				SAssignNew(NamespaceComboBox,SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&NamespaceComboBoxOptions)
				.OnSelectionChanged(this, &SFluidNarratorLocalizationTextsSlateWidget::HandelNamespaceComboBoxSelectionChanged)
				.OnGenerateWidget(this, &SFluidNarratorLocalizationTextsSlateWidget::HandelNamespaceComboBoxMakeWidget)
				.InitiallySelectedItem(NamespaceComboBoxOptionSelected)
				[
					SNew(STextBlock)
					.Text(this, &SFluidNarratorLocalizationTextsSlateWidget::HandelNamespaceComboBoxCurrentItemLabel)
				]
			]
			
			//Culture ComboBox
			+ SHorizontalBox::Slot()
			.Padding(FMargin(4,4))
			.MaxWidth(135)
			[
				SAssignNew(CultureComboBox,SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&CultureComboBoxOptions)
				.OnSelectionChanged(this, &SFluidNarratorLocalizationTextsSlateWidget::HandelCultureComboBoxSelectionChanged)
				.OnGenerateWidget(this, &SFluidNarratorLocalizationTextsSlateWidget::HandelCultureComboBoxMakeWidget)
				.InitiallySelectedItem(CultureComboBoxOptionSelected)
				[
					SNew(STextBlock)
					.Text(this, &SFluidNarratorLocalizationTextsSlateWidget::HandelCultureComboBoxCurrentItemLabel)
				]
			]
		]

		//Table View
		+SVerticalBox::Slot()
		.FillHeight(1.0)
		[	
			SAssignNew(TextsListView,SListView<TSharedPtr<FluidNarratorLocalizationTextEntry>>)
			.ListItemsSource(&TextsItemsCurrent)
			.OnGenerateRow(this, &SFluidNarratorLocalizationTextsSlateWidget::HandelGenerateCultureRowWidget)
			.SelectionMode(ESelectionMode::Multi)
			.ItemHeight(32)
			.ScrollbarVisibility(EVisibility::Collapsed)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+SHeaderRow::Column("Namespace").DefaultLabel(FText::FromString("Namespace"))
				+SHeaderRow::Column("Culture").DefaultLabel(FText::FromString("Culture"))
				+SHeaderRow::Column("Source").DefaultLabel(FText::FromString("Source"))
				+SHeaderRow::Column("Translation").DefaultLabel(FText::FromString("Translation"))
				+SHeaderRow::Column("AutoTranslated").DefaultLabel(FText::FromString("Auto Translated"))
			)
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*=============================================================================
Fluid Narrator Localization Texts Widget : Main
=============================================================================*/

void SFluidNarratorLocalizationTextsSlateWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	HandelTranslateTextComplete();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelLoad()
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return;
	
	//Empty Text Items
	TextsItemPool.Empty();
	TextsItemsCurrent.Empty();
	
	//Get Asset's Namespace Culture Texts
	TArray<FName> Namespaces;
	TArray<FName> Cultures;
	TArray<FString> Sources;
	TArray<FString> Translations;
	TArray<bool> bIsAutoTranslateds;
	LocalizationAsset->GetLocalizationTexts(Namespaces,Cultures,Sources,Translations,bIsAutoTranslateds);

	//Make Text Items
	for (int i = 0; i < Namespaces.Num(); i++)
	{
		TSharedRef<FluidNarratorLocalizationTextEntry> NewItem = FluidNarratorLocalizationTextEntry::Create();
		NewItem->Namespace = Namespaces[i];
		NewItem->Culture = Cultures[i];
		NewItem->Source = Sources[i];
		NewItem->Translation = Translations[i];
		NewItem->bAutoTranslated = bIsAutoTranslateds[i];
		TextsItemPool.Add(NewItem);
	}

	//Load Combo Box
	HandelNamespaceComboBoxLoad();
	HandelCultureComboBoxLoad();
	HandelRows();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelSave()
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return;

	//Remove Asset's Namespace Culture Texts
	LocalizationAsset->Texts.Empty();

	//Make New Namespace Culture Texts
	TArray<FName> Namespaces;
	TArray<FName> Cultures;
	TArray<FString> Sources;
	TArray<FString> Translations;
	TArray<bool> bIsAutoTranslateds;
	
	for (int i = 0; i < TextsItemPool.Num(); i++)
	{
		Namespaces.Add(TextsItemPool[i]->Namespace);
		Cultures.Add(TextsItemPool[i]->Culture);
		Sources.Add(TextsItemPool[i]->Source);
		Translations.Add(TextsItemPool[i]->Translation);
		bIsAutoTranslateds.Add(TextsItemPool[i]->bAutoTranslated);
	}

	//Set Asset's Namespace Culture Texts
	LocalizationAsset->SetLocalizationTexts(Namespaces,Cultures,Sources,Translations,bIsAutoTranslateds);

	//Reload (We May Have Duplicates That Did Not Save And We Want To Reload To Update Entries In Editor)
	HandelLoad();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelRows()
{
	TextsItemsCurrent.Empty();
	
	for (int i = 0; i < TextsItemPool.Num(); i++)
	{
		if (CultureComboBoxOptionSelected != nullptr && *CultureComboBoxOptionSelected == TextsItemPool[i]->Culture.ToString() && NamespaceComboBoxOptionSelected != nullptr && *NamespaceComboBoxOptionSelected == TextsItemPool[i]->Namespace.ToString())
			TextsItemsCurrent.Add(TextsItemPool[i]);
	}

	TextsListView->RebuildList();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelScan()
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return;
	
	//Cache Variables
	UFluidNarratorScreenplay* Screenplay;
	TMap<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText> NamespaceCultureTextsPrevious = LocalizationAsset->Texts;
	TMap<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText> NamespaceCultureTextsNew;
	FFluidNarratorLocalizationTextPair LocalizationTextPair;
	FFluidNarratorLocalizationText LocalizationText;
	FString Namespace;
	TArray<FString> Texts;

	//Empty Asset Texts
	LocalizationAsset->Texts.Empty();
	
	//Get Screenplays
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;

	//Get All Screenplay Assets
	AssetRegistryModule.Get().GetAssetsByClass(UFluidNarratorScreenplay::StaticClass()->GetFName(), AssetData);
	for (int i = 0; i < AssetData.Num(); i++)
	{
		//Get Screenplay
		Screenplay = Cast<UFluidNarratorScreenplay>(AssetData[i].GetAsset());
		
		if (Screenplay != nullptr)
		{
			//Get Namespace
			FluidNarratorScreenplaySystem::ParseNamespaceText(Screenplay->Text,Namespace);
			if (!LocalizationAsset->Namespaces.Contains(FName(Namespace)))
				continue;

			//Get Texts
			FluidNarratorScreenplaySystem::ParseGetTextsAndResponsesCombined(Screenplay->Text,Texts);
			for (int j = 0; j < Texts.Num(); j++)
			{
				//Make One For Each Culture
				TArray<FFluidNarratorLocalizationCulture> CultureArray;
				LocalizationAsset->Cultures.GenerateValueArray(CultureArray);
				for(const FFluidNarratorLocalizationCulture& Culture : CultureArray)
				{
					//Set Localization Text Pair | Text
					LocalizationTextPair.Namespace = FName(Namespace);
					LocalizationTextPair.Culture = FName(Culture.Language);
					LocalizationTextPair.Source = FName(Texts[j]);
					LocalizationText.Translation = FName(Culture.bIsNativeCulture ? Texts[j] : "None");
					LocalizationText.bIsAutoTranslated = false;
					
					//Add Localization Text Pair | Text
					if (!NamespaceCultureTextsNew.Contains(LocalizationTextPair))
						NamespaceCultureTextsNew.Add(LocalizationTextPair,LocalizationText);
				}
			}
		}
	}

	//Add Previous Texts If Still Being Used
	for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : NamespaceCultureTextsPrevious)
		if (NamespaceCultureTextsNew.Contains(Pair.Key) && Pair.Value.Translation != FName("None"))
			NamespaceCultureTextsNew[Pair.Key] = Pair.Value;

	//Set Namespace Culture Texts
	LocalizationAsset->Texts = NamespaceCultureTextsNew;

	//Load
	HandelLoad();

	//Mark Dirty (Needs To Save)
	LocalizationAsset->MarkPackageDirty();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelTranslateText()
{
	//No Valid Culture Selected
	if (NamespaceComboBoxOptionSelected == nullptr || CultureComboBoxOptionSelected == nullptr)
		return;

	//Create Translator
	Translator = NewObject<UFluidNarratorTranslator>();
	
	//Add Source Texts
	TArray<FString> SourceTexts;
	FString TargetLanguageCode = "";
	for (int i = 0; i < TextsItemsCurrent.Num(); i++)
	{
		if (TextsItemsCurrent[i]->Namespace == FName(*NamespaceComboBoxOptionSelected) && TextsItemsCurrent[i]->Culture == FName(*CultureComboBoxOptionSelected))
			if (TextsItemsCurrent[i]->Translation == "none" || TextsItemsCurrent[i]->Translation == "")
			{
				//Get Target Language Code
				if (SourceTexts.Num() == 0)
				{
					if (LocalizationAsset->Cultures.Contains(TextsItemsCurrent[i]->Culture))
						TargetLanguageCode = LocalizationAsset->Cultures[TextsItemsCurrent[i]->Culture].LanguageCode.ToString();
				}

				//Add To Source Texts To Translate
				SourceTexts.Add(TextsItemsCurrent[i]->Source);
			}
	}
	
	//No Texts To Translate Or No Target Language Code Than Destroy Translator And Return
	if (SourceTexts.Num() == 0 || TargetLanguageCode == "")
	{
		Translator->ConditionalBeginDestroy();
		return;
	}
	
	//Translate Multi
	Translator->TranslateMulti(SourceTexts,"", TargetLanguageCode);
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelTranslateTextComplete()
{
	//Only If Translator Is Valid
	if (Translator == nullptr || !Translator->bIsComplete)
		return;

	//Get Result
	const auto Result = Translator->TranslatorResult;
	Translator->bIsComplete = false;
	
	//Destroy Translator
	Translator->ConditionalBeginDestroy();

	//Do Not Continue If Translate Is Not Successful | No Culture Selected
	if (!Result.bSuccessful || NamespaceComboBoxOptionSelected == nullptr || CultureComboBoxOptionSelected == nullptr)
		return;

	//Add Source Texts
	TArray<FString> SourceTexts;
	for (int i = 0; i < Result.SourceTexts.Num(); i++)
		if (TextsItemsCurrent[i]->Namespace == FName(*NamespaceComboBoxOptionSelected) && TextsItemsCurrent[i]->Culture == FName(*CultureComboBoxOptionSelected))
			if (TextsItemsCurrent[i]->Translation == "none" || TextsItemsCurrent[i]->Translation == "")
			{
				TextsItemsCurrent[i]->Translation = Result.TranslatedTexts[i];
				TextsItemsCurrent[i]->bAutoTranslated = true;
			}
	
	//Rebuilt Text List View
	TextsListView->RebuildList();

	//Mark Dirty (Needs To Save)
	LocalizationAsset->MarkPackageDirty();
}

/*=============================================================================
Fluid Narrator Localization Cultures Entry Slate Widget : Handel Text TableRows
=============================================================================*/

TSharedRef<ITableRow> SFluidNarratorLocalizationTextsSlateWidget::HandelGenerateCultureRowWidget(TSharedPtr<FluidNarratorLocalizationTextEntry> Entry, const TSharedRef<STableViewBase>& TableView)
{
	auto N = SNew(SFluidNarratorLocalizationTextEntry, TableView).Entry(Entry);
	N->Parent = this;
	N->LocalizationAsset = LocalizationAsset;
	return N;
}

/*=============================================================================
Fluid Narrator Localization Texts Widget : Handel Namespace ComboBox
=============================================================================*/

TSharedRef<SWidget> SFluidNarratorLocalizationTextsSlateWidget::HandelNamespaceComboBoxMakeWidget(TSharedPtr<FString> InOption) const
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelNamespaceComboBoxSelectionChanged(TSharedPtr<FString> SelectedValue, ESelectInfo::Type)
{
	NamespaceComboBoxOptionSelected = SelectedValue;
	HandelRows();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelNamespaceComboBoxLoad()
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr || LocalizationAsset->Namespaces.Num() == 0)
		return;

	//Set Previous
	NamespaceComboBoxOptionSelectedPrevious = NamespaceComboBoxOptionSelected;
	
	//Set NONE If No Namespaces
	if (LocalizationAsset->Namespaces.Num() == 0)
	{
		NamespaceComboBoxOptions = { (MakeShareable(new FString("No Namespaces"))) };
		NamespaceComboBoxOptionSelected = NamespaceComboBoxOptions[0];
		return;
	}
	
	//Add Namespaces
	NamespaceComboBoxOptions.Empty();
	for (int i = 0; i < LocalizationAsset->Namespaces.Num(); i++)
		NamespaceComboBoxOptions.Add((MakeShareable(new FString (LocalizationAsset->Namespaces[i].ToString()))));

	//Set Selected
	NamespaceComboBoxOptionSelected = NamespaceComboBoxOptions[0];
}

FText SFluidNarratorLocalizationTextsSlateWidget::HandelNamespaceComboBoxCurrentItemLabel() const
{
	if (NamespaceComboBoxOptionSelected.IsValid())
	{
		return FText::FromString(*NamespaceComboBoxOptionSelected);
	}

	return FText::FromString("NULL");
}

/*=============================================================================
Fluid Narrator Localization Texts Widget : Handel Culture ComboBox
=============================================================================*/

TSharedRef<SWidget> SFluidNarratorLocalizationTextsSlateWidget::HandelCultureComboBoxMakeWidget(TSharedPtr<FString> InOption) const
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelCultureComboBoxSelectionChanged(TSharedPtr<FString> SelectedValue, ESelectInfo::Type)
{
	CultureComboBoxOptionSelected = SelectedValue;
	HandelRows();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelCultureComboBoxLoad()
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr || LocalizationAsset->Cultures.Num() == 0)
		return;
	
	//Set Previous
	CultureComboBoxOptionSelectedPrevious = CultureComboBoxOptionSelected;
	
	//Set NONE If No Cultures
	if (LocalizationAsset->Cultures.Num() == 0)
	{
		CultureComboBoxOptions = { (MakeShareable(new FString("No Cultures"))) };
		CultureComboBoxOptionSelected = CultureComboBoxOptions[0];
		return;
	}
	
	//Add Cultures
	CultureComboBoxOptions.Empty();

	TArray<FFluidNarratorLocalizationCulture> CultureArray;
	LocalizationAsset->Cultures.GenerateValueArray(CultureArray);
	for (int i = 0; i <CultureArray.Num(); i++)
		CultureComboBoxOptions.Add((MakeShareable(new FString (CultureArray[i].Language.ToString()))));
	
	//Update Selected
	CultureComboBoxOptionSelected = CultureComboBoxOptions[0];
}

FText SFluidNarratorLocalizationTextsSlateWidget::HandelCultureComboBoxCurrentItemLabel() const
{
	if (CultureComboBoxOptionSelected.IsValid())
	{
		return FText::FromString(*CultureComboBoxOptionSelected);
	}

	return FText::FromString("NULL");
}

/*=============================================================================
Fluid Narrator Localization Texts Widget : Handel Files Import Export
=============================================================================*/

void SFluidNarratorLocalizationTextsSlateWidget::HandelFilesExport()
{
	HandelScan();
	if (LocalizationAsset) LocalizationAsset->FilesExportJson();
	if (LocalizationAsset) LocalizationAsset->FilesExportTsv();
}

void SFluidNarratorLocalizationTextsSlateWidget::HandelFilesImport()
{
	if (LocalizationAsset) LocalizationAsset->FilesImportJson();
	if (LocalizationAsset) LocalizationAsset->FilesImportTsv();
	HandelScan();
	Editor->CulturesWidget->HandelLoad();
}

/*=============================================================================
Fluid Narrator Localization Cultures Entry Slate Widget : Construct
=============================================================================*/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFluidNarratorLocalizationTextEntry::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& TableView )
{
	Entry = InArgs._Entry;
	FSuperRowType::Construct(FSuperRowType::FArguments().Padding(FMargin(0,6)), TableView);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*=============================================================================
Fluid Narrator Localization Cultures Entry Slate Widget : Main
=============================================================================*/

TSharedRef<SWidget> SFluidNarratorLocalizationTextEntry::GenerateWidgetForColumn(const FName& ColumnType)
{
	//Get Font
	const FSlateFontInfo Font = FCoreStyle::Get().GetFontStyle(TEXT("NormalFont"));
	
	//Create Column
	if (ColumnType == "Namespace")
		return SAssignNew(NamespaceEditableText,STextBlock).Font(Font).Text(FText::FromString(Entry->Namespace.ToString())).IsEnabled(false);;
	if (ColumnType == "Culture")
		return SAssignNew(CultureEditableText,STextBlock).Font(Font).Text(FText::FromString(Entry->Culture.ToString())).IsEnabled(false);;
	if (ColumnType == "Source")
		return SAssignNew(SourceEditableText,STextBlock).Font(Font).Text(FText::FromString(Entry->Source)).IsEnabled(false);
	if (ColumnType == "Translation")
		return SAssignNew(TranslationEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Translation)).OnTextChanged(this,&SFluidNarratorLocalizationTextEntry::HandleTextEditorTextChanged);;
	if (ColumnType == "AutoTranslated")
		return SAssignNew(AutoTranslationTextBlock,STextBlock).Font(Font).Text(Entry->bAutoTranslated ? FText::FromString("*") : FText::FromString(" ")).IsEnabled(false);
	return SNew(STextBlock).Font(Font).Text(FText::FromString("  "));
}

void SFluidNarratorLocalizationTextEntry::HandleTextEditorTextChanged(const FText& NewText) const
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return;

	//Update Entry
	Entry->Namespace = FName(NamespaceEditableText->GetText().ToString());
	Entry->Culture = FName(CultureEditableText->GetText().ToString());
	Entry->Source = SourceEditableText->GetText().ToString();
	Entry->Translation = TranslationEditableText->GetText().ToString();
	Entry->bAutoTranslated = false;
	AutoTranslationTextBlock->SetText(Entry->bAutoTranslated ? FText::FromString("*") : FText::FromString(" "));
	
	//Mark Dirty (Needs To Save)
	LocalizationAsset->MarkPackageDirty();
}