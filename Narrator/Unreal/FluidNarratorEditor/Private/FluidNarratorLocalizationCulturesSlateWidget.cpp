// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidNarratorLocalizationCulturesSlateWidget.h"
#include "SlateOptMacros.h"
#include "StatusBar/Private/SStatusBar.h"

/*=============================================================================
Fluid Narrator Localization Cultures Widget : Construct
=============================================================================*/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFluidNarratorLocalizationCulturesSlateWidget::Construct(const FArguments& InArgs)
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
			
			//Add Culture
			+SHorizontalBox::Slot()
			.Padding(FMargin(0,4))
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("StatusBar.StatusBarButton"))
				.OnPressed(this,&SFluidNarratorLocalizationCulturesSlateWidget::AddCulture)
				[
					SNew( SHorizontalBox )
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(FLinearColor::Gray))
						.Font( FEditorStyle::Get().GetFontStyle( "FontAwesome.11" ) )
						.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
						.Text( FText::FromString(FString(TEXT("\xf055"))))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
						.Text(FText::FromString(" Add "))
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
			
			//Remove Culture
			+SHorizontalBox::Slot()
			.Padding(FMargin(0,4))
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("StatusBar.StatusBarButton"))
				.OnPressed(this,&SFluidNarratorLocalizationCulturesSlateWidget::RemoveCulture)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(FLinearColor::Gray))
						.Font( FEditorStyle::Get().GetFontStyle( "FontAwesome.11" ) )
						.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
						.Text( FText::FromString(FString(TEXT("\xf056"))))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock)
						.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
						.Text(FText::FromString(" Remove "))
					]
				]
			]
		]

		//Table View
		+SVerticalBox::Slot()
		.FillHeight(1.0)
		[	
			SAssignNew(CulturesListView,SListView<TSharedPtr<FluidNarratorLocalizationCultureEntry>>)
			.ListItemsSource(&CulturesItems)
			.OnGenerateRow(this, &SFluidNarratorLocalizationCulturesSlateWidget::HandelGenerateCultureRowWidget)
			.SelectionMode(ESelectionMode::Multi)
			.ItemHeight(32)
			.ScrollbarVisibility(EVisibility::Collapsed)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+SHeaderRow::Column("Native").DefaultLabel(FText::FromString("Native"))
				+SHeaderRow::Column("Language").DefaultLabel(FText::FromString("Language"))
				+SHeaderRow::Column("NativeName").DefaultLabel(FText::FromString("Native Name"))
				+SHeaderRow::Column("LanguageCode").DefaultLabel(FText::FromString("Language Code"))
				+SHeaderRow::Column("TranslatorAgency").DefaultLabel(FText::FromString("Translator Agency"))
				+SHeaderRow::Column("Translator").DefaultLabel(FText::FromString("Translator"))
				+SHeaderRow::Column("Version").DefaultLabel(FText::FromString("Version"))
				+SHeaderRow::Column("Date").DefaultLabel(FText::FromString("Date"))
			)
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*=============================================================================
Fluid Narrator Localization Cultures Widget : Main
=============================================================================*/

void SFluidNarratorLocalizationCulturesSlateWidget::HandelLoad()
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return;
	
	//Empty Culture Items
	CulturesItems.Empty();

	//Load Cultures
	TArray<FFluidNarratorLocalizationCulture> CultureArray;
	LocalizationAsset->Cultures.GenerateValueArray(CultureArray);
	for (int i = 0; i < CultureArray.Num(); i++)
	{
		TSharedRef<FluidNarratorLocalizationCultureEntry> NewItem = MakeShareable(new FluidNarratorLocalizationCultureEntry());
		NewItem->Culture = CultureArray[i];
		CulturesItems.Add(NewItem);
	}

	//Refresh
	CulturesListView->RebuildList();;
}

void SFluidNarratorLocalizationCulturesSlateWidget::Save()
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return;

	//Empty Culture
	LocalizationAsset->Cultures.Empty();
	
	//Save Cultures
	for (int i = 0; i < CulturesItems.Num(); i++)
		LocalizationAsset->Cultures.Add(CulturesItems[i]->Culture.Language,CulturesItems[i]->Culture);
}

void SFluidNarratorLocalizationCulturesSlateWidget::AddCulture()
{
	//Add New Culture
	const TSharedRef<FluidNarratorLocalizationCultureEntry> NewItem = MakeShareable(new FluidNarratorLocalizationCultureEntry());
	NewItem->Culture = FFluidNarratorLocalizationCulture();
	NewItem->Culture.NativeName = "NEW";
	NewItem->Culture.Language = "NEW";
	NewItem->Culture.LanguageCode = "NEW";
	NewItem->Culture.Translator = "NEW";
	NewItem->Culture.Version = "NEW";
	NewItem->Culture.Date = "NEW";
	CulturesItems.Add(NewItem);
	CulturesListView->RebuildList();
	
	//Mark Dirty (Needs To Save)
	LocalizationAsset->MarkPackageDirty();
}

void SFluidNarratorLocalizationCulturesSlateWidget::RemoveCulture()
{
	//Remove Selected Cultures
	for (int i = 0; i < CulturesListView->GetSelectedItems().Num(); i++)
	{
		bool bDidDelete = false;
		for(int j = CulturesItems.Num() - 1; j > 0; j--)
		{
			if (CulturesListView->GetSelectedItems()[i]->Culture.Language == CulturesItems[j]->Culture.Language)
			{
				CulturesItems.RemoveAt(j);
				break;
			}
		}
	}
	CulturesListView->RebuildList();

	//Mark Dirty (Needs To Save)
	LocalizationAsset->MarkPackageDirty();
}

TSharedRef<ITableRow> SFluidNarratorLocalizationCulturesSlateWidget::HandelGenerateCultureRowWidget(TSharedPtr<FluidNarratorLocalizationCultureEntry> Entry, const TSharedRef<STableViewBase>& TableView)
{
	auto R = SNew(SFluidNarratorLocalizationCultureEntry, TableView).Entry(Entry);
	R->LocalizationAsset = LocalizationAsset;
	R->Parent = this;
	return R;
}

/*=============================================================================
Fluid Narrator Localization Cultures Entry Slate Widget : Construct
=============================================================================*/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFluidNarratorLocalizationCultureEntry::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& TableView )
{
	Entry = InArgs._Entry;
	FSuperRowType::Construct(FSuperRowType::FArguments().Padding(FMargin(0,6)), TableView);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*=============================================================================
Fluid Narrator Localization Cultures Entry Slate Widget : Main
=============================================================================*/

TSharedRef<SWidget> SFluidNarratorLocalizationCultureEntry::GenerateWidgetForColumn(const FName& ColumnType)
{
	//Get Font
	const FSlateFontInfo Font = FCoreStyle::Get().GetFontStyle(TEXT("NormalFont"));
	
	//Create Column
	if (ColumnType == "Native")
		return SAssignNew(IsNativeNativeLanguageTextBlock,STextBlock).Font(Font).Text(Entry->Culture.bIsNativeCulture ? FText::FromString("*") : FText::FromString(" ")).OnDoubleClicked(this,&SFluidNarratorLocalizationCultureEntry::HandleNativeDoubleClicked);
	if (ColumnType == "Language")
		return SAssignNew(LanguageEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Culture.Language.ToString())).OnTextChanged(this,&SFluidNarratorLocalizationCultureEntry::HandleTextChanged);
	if (ColumnType == "NativeName")
		return SAssignNew(NativeNameEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Culture.NativeName.ToString())).OnTextChanged(this,&SFluidNarratorLocalizationCultureEntry::HandleTextChanged);
	if (ColumnType == "LanguageCode")
		return SAssignNew(LanguageCodeEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Culture.LanguageCode.ToString())).OnTextChanged(this,&SFluidNarratorLocalizationCultureEntry::HandleTextChanged);
	if (ColumnType == "TranslatorAgency")
		return SAssignNew(TranslatorAgencyEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Culture.TranslatorAgency.ToString())).OnTextChanged(this,&SFluidNarratorLocalizationCultureEntry::HandleTextChanged);
	if (ColumnType == "Translator")
		return SAssignNew(TranslatorEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Culture.Translator.ToString())).OnTextChanged(this,&SFluidNarratorLocalizationCultureEntry::HandleTextChanged);
	if (ColumnType == "Version")
		return SAssignNew(VersionEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Culture.Version.ToString())).OnTextChanged(this,&SFluidNarratorLocalizationCultureEntry::HandleTextChanged);
	if (ColumnType == "Date")
		return SAssignNew(DateEditableText,SEditableText).Font(Font).Text(FText::FromString(Entry->Culture.Date.ToString())).OnTextChanged(this,&SFluidNarratorLocalizationCultureEntry::HandleTextChanged);
	return SNew(STextBlock).Font(Font).Text(FText::FromString(""));
}

FReply SFluidNarratorLocalizationCultureEntry::HandleNativeDoubleClicked(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return FReply::Handled();

	//Set All Cultures In Parent To Not Native
	for (int i = 0; i < Parent->CulturesItems.Num(); i++)
		Parent->CulturesItems[i]->Culture.bIsNativeCulture = false;

	//Set Current Culture To Native
	Entry->Culture.bIsNativeCulture = true;

	//Rebuild
	Parent->CulturesListView->RebuildList();
	
	//Mark Dirty (Needs To Save)
	LocalizationAsset->MarkPackageDirty();

	//Return Reply
	return FReply::Handled();
}

void SFluidNarratorLocalizationCultureEntry::HandleTextChanged(const FText& NewText) const
{
	//Return If Not Valid
	if (LocalizationAsset == nullptr)
		return;

	//Set Culture Data
	Entry->Culture.Language = FName(LanguageEditableText->GetText().ToString());
	Entry->Culture.NativeName = FName(NativeNameEditableText->GetText().ToString());
	Entry->Culture.LanguageCode = FName(LanguageCodeEditableText->GetText().ToString());
	Entry->Culture.TranslatorAgency = FName(TranslatorAgencyEditableText->GetText().ToString());
	Entry->Culture.Translator = FName(TranslatorEditableText->GetText().ToString());
	Entry->Culture.Version = FName(VersionEditableText->GetText().ToString());
	Entry->Culture.Date = FName(DateEditableText->GetText().ToString());
	
	//Mark Dirty (Needs To Save)
	LocalizationAsset->MarkPackageDirty();
}