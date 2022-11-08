// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidNarratorScreenplayScriptViewSlateWidget.h"
#include "SlateOptMacros.h"

/*=============================================================================
Fluid Narrator Screenplay Simulator Widget : Construct
=============================================================================*/

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFluidNarratorScreenplayScriptViewWidget::Construct(const FArguments& InArgs)
{
	UFluidNarratorDeveloperSettings* MarshallerSyntaxDeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();

	TSharedRef<FluidNarratorScreenplayMarshaller> MarshallerRef = FluidNarratorScreenplayMarshaller::Create();
	Marshaller = &MarshallerRef.Get();
	
	ChildSlot
	[
		//Create Text Box
		SAssignNew(TextEditorTextBox,SMultiLineEditableTextBox)
		.BackgroundColor(MarshallerSyntaxDeveloperSettings->BackgroundColorTint)
		.Marshaller(MarshallerRef)
		.OnTextChanged(this,&SFluidNarratorScreenplayScriptViewWidget::HandleTextEditorTextChanged)
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*=============================================================================
Fluid Narrator Screenplay Simulator Widget : Main
=============================================================================*/

void SFluidNarratorScreenplayScriptViewWidget::SetText(FText Text)
{
	TextEditorTextBox->SetText(Text);
}

FText SFluidNarratorScreenplayScriptViewWidget::GetText()
{
	return  TextEditorTextBox->GetText();
}

void SFluidNarratorScreenplayScriptViewWidget::SetIsReadOnly(bool Value)
{
	TextEditorTextBox->SetIsReadOnly(Value);	
}

void SFluidNarratorScreenplayScriptViewWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	//We Are Running 
	if (ScreenplayController->bTaskIsRunnings[Editor->ScreenplayTask])
	{
		//Select Current Line Being Ran
		TextEditorTextBox->SetSearchText(FText::FromString(ScreenplayController->GetCurrentLineText(Editor->ScreenplayTask)));
	}
}

void SFluidNarratorScreenplayScriptViewWidget::HandleTextEditorTextChanged(const FText& NewText)
{
	Editor->Asset->MarkPackageDirty();
}

void SFluidNarratorScreenplayScriptViewWidget::HandelAssetPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	//Save Developer Settings
	UFluidNarratorDeveloperSettings* DeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	DeveloperSettings->SaveConfig();

	//Update Background Colors
	TextEditorTextBox->SetTextBoxBackgroundColor(DeveloperSettings->BackgroundColorTint);
	TextEditorTextBox->SetTextBoxBackgroundColor(DeveloperSettings->BackgroundReadOnlyColorTint);
	TextEditorTextBox->SetTextBoxBackgroundColor(DeveloperSettings->BackgroundColorTint);

	//Update Syntax Colors (Set Text To Itself And Refresh Is Only Way I Got It To Work)
	Marshaller->MakeDirty();
	TextEditorTextBox->SetText(TextEditorTextBox->GetText());
	TextEditorTextBox->Refresh();
	Marshaller->MakeDirty();
}