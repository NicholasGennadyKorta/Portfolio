// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidNarratorScreenplaySlateWidget.h"
#include "SlateOptMacros.h"

/*=============================================================================
Fluid Narrator Screenplay Widget : Construct
=============================================================================*/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFluidNarratorScreenplaySlateWidget::Construct(const FArguments& InArgs)
{
	//Set Orientation Vertical
	//Orientation = EOrientation::Orient_Vertical;
	   
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*=============================================================================
Fluid Narrator Screenplay Widget : Main
=============================================================================*/

void SFluidNarratorScreenplaySlateWidget::OnStartPre()
{
	
}

void SFluidNarratorScreenplaySlateWidget::OnStart(FText LineStarted, int LineStartedIndex)
{
	
}

void SFluidNarratorScreenplaySlateWidget::OnStop()
{
	
}

void SFluidNarratorScreenplaySlateWidget::OnSetTextsAndResponses(TArray<FName> TextsCharacterIDs, TArray<FName> ResponsesCharacterIDs, TArray<FText> Texts, TArray<FText> Responses,  TArray<int> ResponseIndexs, bool bHasAutoResponse)
{
	
}

void SFluidNarratorScreenplaySlateWidget::OnSectionStart(FName Section)
{
	
}

void SFluidNarratorScreenplaySlateWidget::OnEvent(FString Event)
{
	
}
