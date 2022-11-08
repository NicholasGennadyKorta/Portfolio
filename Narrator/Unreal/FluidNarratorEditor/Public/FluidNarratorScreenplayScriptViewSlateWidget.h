// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorEditor.h"
#include "FluidNarratorRuntime.h"
#include "CoreMinimal.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SCanvas.h"

class UFluidNarratorDeveloperSettings;
class FluidNarratorScreenplayMarshaller;
class FluidNarratorScreenplayEditor;

class FLUIDNARRATOREDITOR_API SFluidNarratorScreenplayScriptViewWidget : public  SBorder
{

//Construct	
public:
	
	SLATE_BEGIN_ARGS(SFluidNarratorScreenplayScriptViewWidget) { }
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

//Main
public:

	void SetText(FText Text);
	FText GetText();
	void SetIsReadOnly(bool Value);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	void HandleTextEditorTextChanged(const FText& NewText);
	void HandelAssetPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	
	FluidNarratorScreenplayEditor* Editor;
	FluidNarratorScreenplaySystem* ScreenplayController;
	TSharedPtr<SMultiLineEditableTextBox> TextEditorTextBox;
	FluidNarratorScreenplayMarshaller* Marshaller;
};
