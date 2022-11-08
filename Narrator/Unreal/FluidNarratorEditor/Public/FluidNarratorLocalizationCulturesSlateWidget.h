// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorRuntime.h"
#include "FluidNarratorEditor.h"
#include "FluidNarratorRuntime.h"
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

class FluidNarratorLocalizationEditor;
class UFluidNarratorDeveloperSettings;
class SFluidNarratorLocalizationCultureEntry;

//Fluid Narrator Localization Cultures Entry
class FluidNarratorLocalizationCultureEntry
{
public:
	
	FFluidNarratorLocalizationCulture Culture;
};

//Fluid Narrator Localization Cultures Slate Widget
class FLUIDNARRATOREDITOR_API SFluidNarratorLocalizationCulturesSlateWidget : public SCompoundWidget
{

//Construct	
public:
	
	SLATE_BEGIN_ARGS(SFluidNarratorLocalizationCulturesSlateWidget) { }
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

//Main
public:
	
	void HandelLoad();
	void Save();
	void AddCulture();
	void RemoveCulture();
	TSharedRef<ITableRow> HandelGenerateCultureRowWidget(TSharedPtr<FluidNarratorLocalizationCultureEntry> Entry, const TSharedRef<STableViewBase>& TableView);
	
	UFluidNarratorLocalization* LocalizationAsset;
	FluidNarratorLocalizationEditor* Editor;
	TSharedPtr<SListView<TSharedPtr<FluidNarratorLocalizationCultureEntry>>> CulturesListView;
	TArray<TSharedPtr<FluidNarratorLocalizationCultureEntry>> CulturesItems;
};

//Fluid Narrator Localization Cultures Entry Slate Widget
class SFluidNarratorLocalizationCultureEntry : public SMultiColumnTableRow<TSharedPtr<FluidNarratorLocalizationCultureEntry>>
{

//Construct
public:

	SLATE_BEGIN_ARGS(SFluidNarratorLocalizationCultureEntry): _Entry(){}
	SLATE_ARGUMENT(TSharedPtr<FluidNarratorLocalizationCultureEntry>,Entry)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& TableView);

//Main
public:
	
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnType ) override;
	FReply HandleNativeDoubleClicked(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const;
	void HandleTextChanged(const FText& NewText) const;
	
	UFluidNarratorLocalization* LocalizationAsset;
	SFluidNarratorLocalizationCulturesSlateWidget* Parent;
	TSharedPtr<FluidNarratorLocalizationCultureEntry> Entry;

	TSharedPtr<STextBlock> IsNativeNativeLanguageTextBlock;
	TSharedPtr<SEditableText> LanguageEditableText;
	TSharedPtr<SEditableText> NativeNameEditableText;
	TSharedPtr<SEditableText> LanguageCodeEditableText;
	TSharedPtr<SEditableText> TranslatorAgencyEditableText;
	TSharedPtr<SEditableText> TranslatorEditableText;
	TSharedPtr<SEditableText> VersionEditableText;
	TSharedPtr<SEditableText> DateEditableText;
};
