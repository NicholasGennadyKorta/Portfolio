// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidNarratorScreenplayUserWidget.h"

/*=============================================================================
Fluid Narrator Typewriter
=============================================================================*/

void UFluidNarratorTypewriter::BindScreenplayUserWidget(UFluidNarratorScreenplayUserWidget* ScreenplayUserWidgetIn)
{
	ScreenplayUserWidget = ScreenplayUserWidgetIn;	
}

void UFluidNarratorTypewriter::SetText(const FText Text)
{
	SetTexts({Text});	
}

void UFluidNarratorTypewriter::SetTexts(const TArray<FText> Texts)
{
	TypewriterTexts = Texts;
	TypewriterIntervalCurrent = 0;
	TypewriterStep = 0;
	TypewriterStepState = 0;
	TypewriterTextIndex = 0;
	bTypewriterInjectEndTag = false;
	TypewriterStringCurrentFull = "";
	TypewriterStringCurrentStepped = "";
	bIsTypewriterActive = false;
	bWaitForUserInput = false;
	
	if (Texts.Num() > 0)
	{
		TypewriterStringCurrentFull = Texts[0].ToString();
		bIsTypewriterActive = true;

		//On Text Started (Delay To Ensure Setup Is Complete)
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
		{
			if (ScreenplayUserWidget) ScreenplayUserWidget->OnTypewriterTextStarted();
			if (ScreenplayUserWidget) ScreenplayUserWidget->OnTypewritierTextStep(FText::FromString(TypewriterStringCurrentFull),FText::FromString(TypewriterStringCurrentStepped),FText::FromString(&TypewriterStringCurrentFull[TypewriterStep]),TypewriterStep, TypewriterTextIndex);
		}, 0.001F, false, 0);
	}
}

void UFluidNarratorTypewriter::Step(const float TypewriterInterval)
{
	if (bIsTypewriterActive && TypewriterStep < TypewriterStringCurrentFull.Len())
	{
		//Typewriter Interval Current
		TypewriterIntervalCurrent -= GEngine->GetCurrentPlayWorld()->DeltaTimeSeconds * 10;
		if (TypewriterIntervalCurrent > 0)
			return;
		
		// Pre Step
		{
			//< >
			if (TypewriterStringCurrentFull.GetCharArray()[TypewriterStep] == '<')
			{
				TypewriterStepState = 1;
				TypewriterStep = TypewriterStringCurrentFull.Find(">", ESearchCase::IgnoreCase, ESearchDir::FromStart, TypewriterStep);
				bTypewriterInjectEndTag = !bTypewriterInjectEndTag;
			}
		}

		//Step
		TypewriterStep++;
		bWaitForUserInput = TypewriterStep >= TypewriterStringCurrentFull.Len();
		bIsTypewriterActive = TypewriterStep < TypewriterStringCurrentFull.Len();

		//Text Actions Were Here

		//Check If Completed
		if (TypewriterStep >= TypewriterStringCurrentFull.Len())
		{
			FTimerHandle handle;
			GetWorld()->GetTimerManager().SetTimer(handle, [this]()
				{
					if (TypewriterTextIndex >= TypewriterTexts.Num() - 1)
					{
						if (ScreenplayUserWidget) ScreenplayUserWidget->OnTypewriterTextFinishedFinal(FText::FromString(TypewriterStringCurrentFull));
					}
					else
					{
						if (ScreenplayUserWidget) ScreenplayUserWidget->OnTypewriterTextFinished(FText::FromString(TypewriterStringCurrentFull));
					}

				}, 0.001F, false, 0);
		}

		//Finish Step
		if (!bTypewriterInjectEndTag)
			TypewriterStringCurrentStepped = TypewriterStringCurrentFull.Left(TypewriterStep);

		//Finish Step < >
		else
			TypewriterStringCurrentStepped = TypewriterStringCurrentFull.Left(TypewriterStep) + "</>";

		//On Typewriter Text Step
		if (TypewriterStep < TypewriterStringCurrentFull.Len())
		{
			if (ScreenplayUserWidget)
			{
				const auto TextCurrentStep = TypewriterStringCurrentFull[TypewriterStep];
				ScreenplayUserWidget->OnTypewritierTextStep(FText::FromString(TypewriterStringCurrentFull),FText::FromString(TypewriterStringCurrentStepped),FText::FromString(&TextCurrentStep),TypewriterStep, TypewriterTextIndex);
			}
		}
		
		//Finish
		TypewriterIntervalCurrent = TypewriterInterval;
	}
}

void UFluidNarratorTypewriter::Input(const int PlayerIndex, const TArray<FKey> NextKeys, bool& bDidInput_Out)
{
	//We Are Not Waiting For User Input
	if (!bWaitForUserInput)
		return;
		
	//Get Player Controller
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),PlayerIndex);
	if (PlayerController == nullptr)
		return;

	//Get If Input Was Done
	bDidInput_Out = false;
	for (int i = 0; i < NextKeys.Num(); i++)
		if (PlayerController->WasInputKeyJustPressed(NextKeys[i]))
			bDidInput_Out = true;
	if (!bDidInput_Out) return;
	bDidInput_Out = true;
	
	//Go To Next Text Index
	TypewriterTextIndex++;
	
	//Go To Next Text (Not Last)
	if (TypewriterTextIndex < TypewriterTexts.Num())
	{
		TypewriterIntervalCurrent = 0;
		TypewriterStepState = 0;
		bTypewriterInjectEndTag = false;
		bWaitForUserInput = false;
		TypewriterStep = 0;
    
		TypewriterStringCurrentFull = TypewriterTexts[TypewriterTextIndex].ToString();
		bIsTypewriterActive = true;
			
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
			{
				if (ScreenplayUserWidget) ScreenplayUserWidget->OnTypewriterTextContinued(TypewriterTexts[TypewriterTextIndex],TypewriterTextIndex);
			}, 0.001F, false, 0);
	}
    
	//Go To Next Text (Last)
	else
	{
		bIsTypewriterActive = false;
		bWaitForUserInput = false;
			
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
			{
				if (ScreenplayUserWidget) ScreenplayUserWidget->OnTypewriterTextContinuedFinal(TypewriterTexts[TypewriterTextIndex - 1],TypewriterTextIndex - 1);
			}, 0.001F, false, 0);

		//End Text Actions Were Here
	}
}

/*=============================================================================
Fluid Narrator Screenplay Response User Widget
=============================================================================*/

void UFluidNarratorScreenplayResponseUserWidget::BindScreenplayUserWidget(UFluidNarratorScreenplayUserWidget* ScreenplayUserWidgetIn)
{
	ScreenplayUserWidget = ScreenplayUserWidgetIn;	
}

void UFluidNarratorScreenplayResponseUserWidget::NativeConstruct()
{
	//Call Parent Native Construct
	Super::NativeConstruct();
	
	// Enable Tick For C++ Construct
	// Disabled By Default, Unless Animations / Etc, See UserWidget.h:45
	TSharedPtr<SObjectWidget> SafeGCWidget = MyGCWidget.Pin();
	if (SafeGCWidget.IsValid())
		SafeGCWidget->SetCanTick(true);
}

void UFluidNarratorScreenplayResponseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Tick
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFluidNarratorScreenplayResponseUserWidget::Input(const int PlayerIndex, const TArray<FKey> KeysNext, const TArray<FKey> KeysPrevious, const TArray<FKey> KeysSelect, bool& bDidSelectInput_Out)
{
	//Default Outs
	bDidSelectInput_Out = false;
	
	//We Are Not Enabled Or No Responses
	if (!bIsEnabled || Responses.Num() <= 0)
		return;
		
	//Get Player Controller
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),PlayerIndex);
	if (PlayerController == nullptr)
		return;

	//Get If Input Next
	for (int i = 0; i < KeysNext.Num(); i++)
		if (PlayerController->WasInputKeyJustPressed(KeysNext[i]))
		{
			const int ResponseSelectedIndexPrevious = ResponseSelectedIndex;
			ResponseSelectedIndex++;
			ResponseSelectedIndex = FMath::Clamp(ResponseSelectedIndex,0,Responses.Num() - 1);
			if (ResponseSelectedIndex != ResponseSelectedIndexPrevious)
			{
				ResponseItemUserWidgets[ResponseSelectedIndexPrevious]->OnHoveredEnd();
				ResponseItemUserWidgets[ResponseSelectedIndex]->OnHoveredStart();
			}
			return;
		}
		

	//Get If Input Previous
	for (int i = 0; i < KeysPrevious.Num(); i++)
		if (PlayerController->WasInputKeyJustPressed(KeysPrevious[i]))
		{
			const int ResponseSelectedIndexPrevious = ResponseSelectedIndex;
			ResponseSelectedIndex--;
			ResponseSelectedIndex = FMath::Clamp(ResponseSelectedIndex,0,Responses.Num() - 1);
			if (ResponseSelectedIndex != ResponseSelectedIndexPrevious)
			{
				ResponseItemUserWidgets[ResponseSelectedIndexPrevious]->OnHoveredEnd();
				ResponseItemUserWidgets[ResponseSelectedIndex]->OnHoveredStart();
			}
			return;
		}

	//Get If Input Select
	for (int i = 0; i < KeysSelect.Num(); i++)
		if (PlayerController->WasInputKeyJustPressed(KeysSelect[i]))
		{
			bDidSelectInput_Out = true;
			ResponseItemUserWidgets[ResponseSelectedIndex]->OnSelected();
			if (ScreenplayUserWidget) ScreenplayUserWidget->OnResponseSelected(ResponsesCharacterIDs[ResponseSelectedIndex],Responses[ResponseSelectedIndex],ResponseIndexs[ResponseSelectedIndex]);
			return;
		}
}

void UFluidNarratorScreenplayResponseUserWidget::SetEnabled()
{
	//Set Enabled
	bIsEnabled = true;
	for (int i = 0; i < ResponseItemUserWidgets.Num(); i++)
		ResponseItemUserWidgets[i]->OnEnabled();
}

void UFluidNarratorScreenplayResponseUserWidget::SetDisabled()
{
	//Set Disabled
	bIsEnabled = false;
	for (int i = 0; i < ResponseItemUserWidgets.Num(); i++)
		ResponseItemUserWidgets[i]->OnDisabled();
}

void UFluidNarratorScreenplayResponseUserWidget::SendEvent(const FString Event)
{
	//On Event
	OnEvent(Event);
	for (int i = 0; i < ResponseItemUserWidgets.Num(); i++)
		ResponseItemUserWidgets[i]->OnEvent(Event);
}

void UFluidNarratorScreenplayResponseUserWidget::SetResponses(TSubclassOf<UFluidNarratorScreenplayResponseItemUserWidget> ResponseItemUserWidgetClass, const TArray<FName> ResponsesCharacterIDsIn, const TArray<FText> ResponsesIn, const TArray<int> ResponseIndexsIn, const bool bHasAutoResponseIn, const ESlateVisibility DefaultVisibility)
{
	//Not Valid Input
	if (PanelWidget == nullptr || ResponsesIn.Num() != ResponsesCharacterIDsIn.Num() ||  ResponsesIn.Num() != ResponseIndexsIn.Num())
		return;
	
	//Set Data
	ResponsesCharacterIDs = ResponsesCharacterIDsIn;
	Responses = ResponsesIn;
	ResponseIndexs = ResponseIndexsIn;
	bHasAutoResponse = bHasAutoResponseIn;
	ResponseSelectedIndex = 0;
	SetDisabled();

	//Get Min Response Length
	int MinResponseCharLength = 0;
	for (int i = 0; i < Responses.Num(); i++)
		MinResponseCharLength = Responses[i].ToString().Len();
	
	//Create Response Item User Widgets
	ResponseItemUserWidgets.Empty();
	PanelWidget->ClearChildren();	
	for (int i = 0; i < Responses.Num(); i++)
	{
		//Create Screenplay Widget
		UFluidNarratorScreenplayResponseItemUserWidget* Widget = CreateWidget<UFluidNarratorScreenplayResponseItemUserWidget>(GetWorld(),ResponseItemUserWidgetClass);
		Widget->OnResponseItemConstructed(ResponsesCharacterIDs[i],Responses[i],ResponseIndexs[i],DefaultVisibility, MinResponseCharLength);
		if (i == 0) Widget->OnHoveredConstructed();
		ResponseItemUserWidgets.Add(Widget);
		PanelWidget->AddChild(Widget);
	}
}

/*=============================================================================
Fluid Narrator Screenplay Response Item User Widget
=============================================================================*/

void UFluidNarratorScreenplayResponseItemUserWidget::NativeConstruct()
{
	//Call Parent Native Construct
	Super::NativeConstruct();
	
	// Enable Tick For C++ Construct
	// Disabled By Default, Unless Animations / Etc, See UserWidget.h:45
	TSharedPtr<SObjectWidget> SafeGCWidget = MyGCWidget.Pin();
	if (SafeGCWidget.IsValid())
		SafeGCWidget->SetCanTick(true);
}

void UFluidNarratorScreenplayResponseItemUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Tick
	Super::NativeTick(MyGeometry, InDeltaTime);
}

/*=============================================================================
Fluid Narrator Screenplay User Widget
=============================================================================*/

void UFluidNarratorScreenplayUserWidget::NativeConstruct()
{
	//Call Parent Native Construct
	Super::NativeConstruct();

	// Enable Tick For C++ Construct
	// Disabled By Default, Unless Animations / Etc, See UserWidget.h:45
	TSharedPtr<SObjectWidget> SafeGCWidget = MyGCWidget.Pin();
	if (SafeGCWidget.IsValid())
		SafeGCWidget->SetCanTick(true);
}

void UFluidNarratorScreenplayUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Tick
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFluidNarratorScreenplayUserWidget::SetScreenplayTaskWaitingForTextOrResponseFalse()
{
	if (ScreenplaySystem)
	{
		ScreenplaySystem->StopAutoResponse(ScreenplayTask,ScreenplayTask);
		ScreenplaySystem->bTaskIsWaitingForTextAndResponses[ScreenplayTask] = false;
	}
}

void UFluidNarratorScreenplayUserWidget::SetScreenplayTaskWaitingForTextOrResponseFalseIfNoTextAndResponses(const TArray<FText> Texts, const TArray<FText> Responses)
{
	if (ScreenplaySystem && Texts.Num() == 0 && Responses.Num() == 0)
	{
		ScreenplaySystem->StopAutoResponse(ScreenplayTask,ScreenplayTask);
		ScreenplaySystem->bTaskIsWaitingForTextAndResponses[ScreenplayTask] = false;
	}
}

void UFluidNarratorScreenplayUserWidget::SetScreenplayTaskWaitingForEventFalse()
{
	if (ScreenplaySystem)
		ScreenplaySystem->bTaskIsWaitingForEvents[ScreenplayTask] = false;
}

void UFluidNarratorScreenplayUserWidget::SetScreenplayTaskResponseIndex(const int ResponseIndex)
{
	if (ScreenplaySystem)
	{
		ScreenplaySystem->StopAutoResponse(ScreenplayTask,ScreenplayTask);
		ScreenplaySystem->bTaskIsWaitingForTextAndResponses[ScreenplayTask] = false;
		ScreenplaySystem->TaskResponseIndexs[ScreenplayTask] = ResponseIndex;
	}
}

void UFluidNarratorScreenplayUserWidget::SetScreenplayTaskAutoResponseEnabledIfPossible()
{
	//Enable Auto Response If There Is A Auto Response
	if (ScreenplaySystem && ScreenplaySystem->TasksAutoResponseComponents[ScreenplayTask].TimerStart > 0)
		ScreenplaySystem->TasksAutoResponseComponents[ScreenplayTask].bEnabled = true;
}

void UFluidNarratorScreenplayUserWidget::GetScreenplayTaskAutoResponseTimeRemainingPercentage(float& Percentage_Out, bool& bIsDoingAutoResponse_Out)
{
	//Default Outs
	Percentage_Out = 0;
	bIsDoingAutoResponse_Out = false;

	//Set Outs
	if (ScreenplaySystem && ScreenplaySystem->TasksAutoResponseComponents[ScreenplayTask].bEnabled)
	{
		Percentage_Out = ScreenplaySystem->TasksAutoResponseComponents[ScreenplayTask].TimerCurrent /  ScreenplaySystem->TasksAutoResponseComponents[ScreenplayTask].TimerStart;
		bIsDoingAutoResponse_Out = true;
	}
}