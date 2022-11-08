// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorRuntime.h"
#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Slate/SObjectWidget.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"
#include "FluidNarratorScreenplayUserWidget.generated.h"

/**
 * 
 */

class FluidNarratorScreenplaySystem;
class UFluidNarratorScreenplayUserWidget;
class UFluidNarratorScreenplayUserResponseWidget;
class UFluidNarratorScreenplayResponseItemUserWidget;

//Fluid Narrator Typewriter
UCLASS(BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorTypewriter : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
		void BindScreenplayUserWidget(UFluidNarratorScreenplayUserWidget* ScreenplayUserWidgetIn);
	UFUNCTION(BlueprintCallable)
		void SetText(const FText Text);
	UFUNCTION(BlueprintCallable)
		void SetTexts(const TArray<FText> Texts);
	UFUNCTION(BlueprintCallable)	
		void Step(const float TypewriterInterval);
	UFUNCTION(BlueprintCallable)	
		void Input(const int PlayerIndex, const TArray<FKey> NextKeys, bool& bDidInput_Out);

	UPROPERTY(BlueprintReadOnly)
		UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget;
	UPROPERTY(BlueprintReadOnly)
		TArray<FText> TypewriterTexts;
	UPROPERTY(BlueprintReadOnly)
		FString TypewriterStringCurrentFull;
	UPROPERTY(BlueprintReadOnly)
		FString TypewriterStringCurrentStepped;
	UPROPERTY(BlueprintReadOnly)
		float TypewriterIntervalDefault;
	UPROPERTY(BlueprintReadOnly)
		float TypewriterIntervalCurrent;
	UPROPERTY(BlueprintReadOnly)
		int TypewriterStep;
	UPROPERTY(BlueprintReadOnly)
		int TypewriterStepState;
	UPROPERTY(BlueprintReadOnly)
		int TypewriterTextIndex;
	UPROPERTY(BlueprintReadOnly)
		bool bTypewriterInjectEndTag;
	UPROPERTY(BlueprintReadOnly)
		bool bIsTypewriterActive;
	UPROPERTY(BlueprintReadOnly)
		bool bWaitForUserInput;
};

//Fluid Narrator Screenplay Response User Widget
UCLASS(Blueprintable,BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorScreenplayResponseUserWidget : public UUserWidget
{
	GENERATED_BODY()

//Main
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void BindScreenplayUserWidget(UFluidNarratorScreenplayUserWidget* ScreenplayUserWidgetIn);
	UFUNCTION(BlueprintCallable)	
		void Input(const int PlayerIndex, const TArray<FKey> KeysNext, const TArray<FKey> KeysPrevious, const TArray<FKey> KeysSelect, bool& bDidSelectInput_Out);
	UFUNCTION(BlueprintCallable)
		void SetEnabled();
	UFUNCTION(BlueprintCallable)
		void SetDisabled();
	UFUNCTION(BlueprintCallable)
		void SendEvent(const FString Event);
	UFUNCTION(BlueprintCallable)
		void SetResponses(TSubclassOf<UFluidNarratorScreenplayResponseItemUserWidget> ResponseItemUserWidgetClass, const TArray<FName> ResponsesCharacterIDsIn, const TArray<FText> ResponsesIn, const TArray<int> ResponseIndexsIn, const bool bHasAutoResponseIn, const ESlateVisibility DefaultVisibility);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnEnabled();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnDisabled();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnEvent(const FString& Event);
	
	UPROPERTY(BlueprintReadWrite)
		UPanelWidget* PanelWidget;
	UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget;
	TArray<UFluidNarratorScreenplayResponseItemUserWidget*> ResponseItemUserWidgets;
	TArray<FName> ResponsesCharacterIDs;
	TArray<FText> Responses;
	TArray<int> ResponseIndexs;
	int ResponseSelectedIndex;
	bool bHasAutoResponse;
	bool bIsEnabled;
};

//Fluid Narrator Screenplay Response Item User Widget
UCLASS(Blueprintable,BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorScreenplayResponseItemUserWidget : public UUserWidget
{
	GENERATED_BODY()

//Main
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnResponseItemConstructed(const FName& ResponseCharacterID, const FText& Response, const int& ResponseIndex, const ESlateVisibility& DefaultVisibility, const int& MinResponseCharLength);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnSelected();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnHoveredConstructed();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnHoveredStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnHoveredEnd();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnEnabled();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnDisabled();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnEvent(const FString& Event);
};

//Fluid Narrator Screenplay User Widget
UCLASS(Blueprintable,BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorScreenplayUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
//Main
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetScreenplayTaskWaitingForTextOrResponseFalse();
	UFUNCTION(BlueprintCallable)
		void SetScreenplayTaskWaitingForTextOrResponseFalseIfNoTextAndResponses(const TArray<FText> Texts, const TArray<FText> Responses);
	UFUNCTION(BlueprintCallable)
		void SetScreenplayTaskWaitingForEventFalse();
	UFUNCTION(BlueprintCallable)
		void SetScreenplayTaskResponseIndex(const int ResponseIndex);
	UFUNCTION(BlueprintCallable)
		void SetScreenplayTaskAutoResponseEnabledIfPossible();
	UFUNCTION(BlueprintCallable)
		void GetScreenplayTaskAutoResponseTimeRemainingPercentage(float& Percentage_Out, bool& bIsDoingAutoResponse_Out);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnStartPre();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnStart(const FText& LineStarted, const int& LineStartedIndex);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnStop();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnSetTextsAndResponses(const TArray<FName>& TextsCharacterIDs, const TArray<FName>& ResponsesCharacterIDs, const TArray<FText>& Texts, const TArray<FText>& Responses, const TArray<int>& ResponseIndexs, const uint8& bHasAutoResponseAsByte);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnSectionStart(const FName& Section);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnEvent(const FString& Event);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnResponseSelected(const FName& ResponseCharacterID, const FText& ResponseText, const int& ResponseIndex);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnTypewriterTextStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnTypewriterTextContinued(const FText& CurrentTextFull, const int& TextIndex);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnTypewriterTextContinuedFinal(const FText& CurrentTextFull, const int& TextIndex);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnTypewriterTextFinished(const FText& CurrentTextFull);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnTypewriterTextFinishedFinal(const FText& CurrentTextFull);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnTypewritierTextStep(const FText& CurrentTextFull, const FText& CurrentTextStepped, const FText& CurrentTextStep, const int& CurrentStep, const int& TextIndex);

	FluidNarratorScreenplaySystem* ScreenplaySystem;
	UPROPERTY(BlueprintReadOnly)
		int ScreenplayTask;
};