// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorRuntime.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class FluidNarratorScreenplaySystem;

class FLUIDNARRATORRUNTIME_API SFluidNarratorScreenplaySlateWidget : public  SCompoundWidget
{
	
//Construct	
public:
	
	SLATE_BEGIN_ARGS(SFluidNarratorScreenplaySlateWidget){}
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);
	
//Main
public:

	UFUNCTION(BlueprintCallable)
		void virtual OnStartPre();
	UFUNCTION(BlueprintCallable)
		void virtual OnStart(FText LineStarted, int LineStartedIndex);
	UFUNCTION(BlueprintCallable)
		void virtual OnStop();
	UFUNCTION(BlueprintCallable)
		void virtual OnSetTextsAndResponses(TArray<FName> TextsCharacterIDs, TArray<FName> ResponsesCharacterIDs, TArray<FText> Texts, TArray<FText> Responses, TArray<int> ResponseIndexs, bool bHasAutoResponse);
	UFUNCTION(BlueprintCallable)
		void virtual OnSectionStart(FName Section);
	UFUNCTION(BlueprintCallable)
		void virtual OnEvent(FString Event);

	FluidNarratorScreenplaySystem* ScreenplaySystem;
	int ScreenplayTask;
};
