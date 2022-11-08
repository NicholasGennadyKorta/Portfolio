// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorEditor.h"
#include "FluidNarratorRuntime.h"
#include "CoreMinimal.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SCanvas.h"

/**
 * 
 */

class UFluidNarratorDeveloperSettings;
class FluidNarratorScreenplayEditor;

class FLUIDNARRATOREDITOR_API SFluidNarratorScreenplaySimulatorSlateWidget : public  SFluidNarratorScreenplaySlateWidget
{

//Construct	
public:
	
	SLATE_BEGIN_ARGS(SFluidNarratorScreenplaySimulatorSlateWidget) { }
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

//Main
public:
	
	virtual void OnStartPre() override;
	virtual void OnStart(FText LineStarted, int LineStartedIndex) override;
	virtual void OnStop() override;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual void OnSetTextsAndResponses(TArray<FName> TextsCharacterIDs, TArray<FName> ResponsesCharacterIDs, TArray<FText> Texts, TArray<FText> Responses, TArray<int> ResponseIndexs,  bool bHasAutoResponse) override;
	virtual void OnSectionStart(FName Section) override;
	virtual void OnEvent(FString Event) override;

	//TMap<FText,int> ResponseTexts;
	TSharedPtr<SScrollBox> ScrollBox;
	FluidNarratorScreenplayEditor* Editor;

//Handels
public:
	FReply HandelResponseClicked(const int Index);
};
