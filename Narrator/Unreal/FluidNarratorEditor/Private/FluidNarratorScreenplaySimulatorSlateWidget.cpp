// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidNarratorScreenplaySimulatorSlateWidget.h"
#include "SlateOptMacros.h"

/*=============================================================================
Fluid Narrator Screenplay Simulator Widget : Construct
=============================================================================*/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFluidNarratorScreenplaySimulatorSlateWidget::Construct(const FArguments& InArgs)
{
	UFluidNarratorDeveloperSettings* MarshallerSyntaxDeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	
	//Set Padding
	ChildSlot.SetPadding(FMargin(10,10));
	
	//Create Scrollbar
	ScrollBox = SNew(SScrollBox)
	.Orientation(EOrientation::Orient_Vertical);
	ChildSlot.AttachWidget(ScrollBox.ToSharedRef());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*=============================================================================
Fluid Narrator Screenplay Simulator Widget : Main
=============================================================================*/

void SFluidNarratorScreenplaySimulatorSlateWidget::OnStartPre()
{
	//Clear Children
	if (ScrollBox)
		ScrollBox->ClearChildren();
}

void SFluidNarratorScreenplaySimulatorSlateWidget::OnStart(FText LineStarted, int LineStartedIndex)
{
	Editor->HandelScreenplayStart();
}

void SFluidNarratorScreenplaySimulatorSlateWidget::OnStop()
{
	Editor->HandelScreenplayStop();
}

void SFluidNarratorScreenplaySimulatorSlateWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	ScreenplaySystem->Tick(InDeltaTime);

	//Extra Screenplay Tick Func
	if (ScreenplaySystem->bTaskIsRunnings[ScreenplayTask])
		Editor->HandelScreenplayTick();
}

void SFluidNarratorScreenplaySimulatorSlateWidget::OnSetTextsAndResponses(TArray<FName> TextsCharacterIDs, TArray<FName> ResponsesCharacterIDs, TArray<FText> Texts, TArray<FText> Responses, TArray<int> ResponseIndexs,  bool bHasAutoResponse)
{
	//Add Texts
	for (int i = 0; i < Texts.Num(); i++)
	{
		FString String = TextsCharacterIDs[i].ToString() + ":" + Texts[i].ToString();
		FluidNarratorScreenplaySystem::ParseVariablesApplyToString(String,ScreenplaySystem->Variables,String);
		
		ScrollBox->AddSlot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(String))
			.IsEnabled(true)
			.RenderOpacity(1)
		];
	}
	
	//Add Responses (As Buttons)
	for (int i = 0; i < Responses.Num(); i++)
	{
		FString String = ResponsesCharacterIDs[i].ToString() + ":" + Responses[i].ToString();
		FluidNarratorScreenplaySystem::ParseVariablesApplyToString(String,ScreenplaySystem->Variables,String);
		
		ScrollBox->AddSlot()
		[
			SNew(SButton)
			.OnClicked(this, &SFluidNarratorScreenplaySimulatorSlateWidget::HandelResponseClicked,ResponseIndexs[i])
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString(String))
				.IsEnabled(true)
				.RenderOpacity(1)
			]
		];
	}

	//Set Not Waiting For Text And Responses If No Responses
	if (ScreenplaySystem && Responses.Num() == 0 && !bHasAutoResponse)
		ScreenplaySystem->bTaskIsWaitingForTextAndResponses[ScreenplayTask] = false;

	if (ScreenplaySystem && bHasAutoResponse)
		ScreenplaySystem->TasksAutoResponseComponents[ScreenplayTask].bEnabled = true;
}

void SFluidNarratorScreenplaySimulatorSlateWidget::OnSectionStart(FName Section)
{
	UFluidNarratorDeveloperSettings* MarshallerSyntaxDeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	
	ScrollBox->AddSlot()
	[
		SNew(STextBlock)
		.Text(FText::FromString(Section.ToString()))
		.ColorAndOpacity(MarshallerSyntaxDeveloperSettings->TextBlockStyleColorSection)
	];
}

void SFluidNarratorScreenplaySimulatorSlateWidget::OnEvent(FString Event)
{
	UFluidNarratorDeveloperSettings* MarshallerSyntaxDeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	
	ScrollBox->AddSlot()
	[
		SNew(STextBlock)
		.Text(FText::FromString(Event))
		.ColorAndOpacity(MarshallerSyntaxDeveloperSettings->TextBlockStyleColorEvent)
	];
	
	//Set Auto Not Waiting For Event
	if (ScreenplaySystem)
		ScreenplaySystem->bTaskIsWaitingForEvents[Editor->ScreenplayTask] = false;
}

/*=============================================================================
Fluid Narrator Screenplay Simulator Widget : Handels
=============================================================================*/

FReply SFluidNarratorScreenplaySimulatorSlateWidget::HandelResponseClicked(int Index)
{
	ScreenplaySystem->StopAutoResponse(ScreenplayTask,ScreenplayTask);
	ScreenplaySystem->bTaskIsWaitingForTextAndResponses[ScreenplayTask] = false;
	ScreenplaySystem->TaskResponseIndexs[ScreenplayTask] = Index;
	return FReply::Handled();
}