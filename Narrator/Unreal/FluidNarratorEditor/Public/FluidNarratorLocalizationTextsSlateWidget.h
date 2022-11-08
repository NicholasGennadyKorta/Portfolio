// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorRuntime.h"
#include "FluidNarratorEditor.h"
#include "FluidNarratorRuntime.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Styling/StyleColors.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/SCanvas.h"
#include "Components/ComboBoxString.h"

/**
 * 
 */

class UFluidNarratorDeveloperSettings;
class SFluidNarratorLocalizationCultureEntry;
class UFluidNarratorTranslator;
class FluidNarratorLocalizationEditor;
class SFluidNarratorLocalizationTextEntry;

//Fluid Narrator Localization Texts Entry
class FluidNarratorLocalizationTextEntry
{
public:
	
	static TSharedRef<FluidNarratorLocalizationTextEntry> Create()
	{
		return MakeShareable( new FluidNarratorLocalizationTextEntry() );
	}
	
	FName Namespace;
	FName Culture;
	FString Source;
	FString Translation;
	bool bAutoTranslated;
};

//Fluid Narrator Localization Texts Slate Widget
class FLUIDNARRATOREDITOR_API SFluidNarratorLocalizationTextsSlateWidget : public SCompoundWidget
{

//Construct	
public:
	
	SLATE_BEGIN_ARGS(SFluidNarratorLocalizationCulturesSlateWidget) { }
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

//Main
public:

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	void HandelLoad();
	void HandelSave();
	void HandelRows();
	void HandelScan();
	void HandelTranslateText();
	void HandelTranslateTextComplete();
	
	UFluidNarratorTranslator* Translator;
	UFluidNarratorLocalization* LocalizationAsset;
	FluidNarratorLocalizationEditor* Editor;

//Handel Text TableRows
public:
	TSharedRef<ITableRow> HandelGenerateCultureRowWidget(TSharedPtr<FluidNarratorLocalizationTextEntry> Entry, const TSharedRef<STableViewBase>& TableView);

	TSharedPtr<SListView<TSharedPtr<FluidNarratorLocalizationTextEntry>>> TextsListView;
	TArray<TSharedPtr<FluidNarratorLocalizationTextEntry>> TextsItemsCurrent;
	TArray<TSharedPtr<FluidNarratorLocalizationTextEntry>> TextsItemPool;
	
//Handel Namespace ComboBox
public:

	TSharedRef<SWidget> HandelNamespaceComboBoxMakeWidget(TSharedPtr<FString> InOption) const;
	void HandelNamespaceComboBoxSelectionChanged(TSharedPtr<FString> SelectedValue, ESelectInfo::Type);
	void HandelNamespaceComboBoxLoad();
	FText HandelNamespaceComboBoxCurrentItemLabel() const;
	
	TSharedPtr<SComboBox<TSharedPtr<FString>>> NamespaceComboBox;
	TArray<TSharedPtr<FString>> NamespaceComboBoxOptions;
	TSharedPtr<FString> NamespaceComboBoxOptionSelected;
	TSharedPtr<FString> NamespaceComboBoxOptionSelectedPrevious;
	
//Handel Culture ComboBox
public:
	TSharedRef<SWidget> HandelCultureComboBoxMakeWidget(TSharedPtr<FString> InOption) const;
	void HandelCultureComboBoxSelectionChanged(TSharedPtr<FString> SelectedValue, ESelectInfo::Type);
	void HandelCultureComboBoxLoad();
	FText HandelCultureComboBoxCurrentItemLabel() const;

	TSharedPtr<SComboBox<TSharedPtr<FString>>> CultureComboBox;
	TArray<TSharedPtr<FString>> CultureComboBoxOptions;
	TSharedPtr<FString> CultureComboBoxOptionSelected;
	TSharedPtr<FString> CultureComboBoxOptionSelectedPrevious;

//Handel File Import Export
public:
	
	void HandelFilesExport();
	void HandelFilesImport();
};

//Fluid Narrator Localization Cultures Entry Slate Widget
class SFluidNarratorLocalizationTextEntry : public SMultiColumnTableRow<TSharedPtr<FluidNarratorLocalizationTextEntry>>
{

//Construct
public:

	SLATE_BEGIN_ARGS(SFluidNarratorLocalizationTextEntry): _Entry(){}
	SLATE_ARGUMENT(TSharedPtr<FluidNarratorLocalizationTextEntry>,Entry)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& TableView);

//Main
public:
	
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnType ) override;
	void HandleTextEditorTextChanged(const FText& NewText) const;

	SFluidNarratorLocalizationTextsSlateWidget* Parent;
	UFluidNarratorLocalization* LocalizationAsset;
	TSharedPtr<FluidNarratorLocalizationTextEntry> Entry;
	TSharedPtr<STextBlock> NamespaceEditableText;
	TSharedPtr<STextBlock> CultureEditableText;
	TSharedPtr<STextBlock> SourceEditableText;
	TSharedPtr<SEditableText> TranslationEditableText;
	TSharedPtr<STextBlock> AutoTranslationTextBlock;
};
