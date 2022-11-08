// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidNarratorSubsystem.h"

/*=============================================================================
Fluid Narrator Subsystem : Main
=============================================================================*/

void UFluidNarratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//Initialize Once
	if (bIsInitialized) return;
	bIsInitialized = true;

	NarratorScreenplaySystem = FluidNarratorScreenplaySystem();
}

void UFluidNarratorSubsystem::Deinitialize()
{
	bIsInitialized = false;
}

void UFluidNarratorSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bIsInitialized) return;

	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidNarrator_Screenplay);
	{
		NarratorScreenplaySystem.Tick(DeltaTime);
	}
}

bool UFluidNarratorSubsystem::IsTickable() const
{
	return !IsTemplate() && bIsInitialized;
}

bool UFluidNarratorSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidNarratorSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidNarratorSubsystem::GetStatId() const
{
	return TStatId();
}

/*=============================================================================
Fluid Narrator Subsystem : Main
=============================================================================*/

void UFluidNarratorSubsystem::StartScreenplayTaskBySectionNameByText(FName CultureOverride, const FString TextAsString, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, const FName SectionName,  UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out)
{
	//Return If Not Valid
	if (!bIsInitialized || ScreenplayUserWidgetClass == nullptr)
		return;

	//Create Screenplay Widget
	UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget = CreateWidget<UFluidNarratorScreenplayUserWidget>(GetWorld(), ScreenplayUserWidgetClass);
	ScreenplayUserWidget->AddToViewport();
	ScreenplayUserWidget_Out = ScreenplayUserWidget;

	//Get Culture (Use Default If Input Is Empty)
	CultureOverride = CultureOverride == "" ? FName(LocalizationCulture.Language) : CultureOverride;
	
	//Start
	NarratorScreenplaySystem.StartBySectionName(CultureOverride, TextAsString,SectionName, ScreenplayUserWidget,nullptr,ScreenplayTask_Out);

	//Task Not Started So Delete User Widget
	if (ScreenplayTask_Out == -1)
		ScreenplayUserWidget->RemoveFromParent();
}

void UFluidNarratorSubsystem::StartScreenplayTaskBySectionAutoByText(FName CultureOverride, const FString TextAsString, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out)
{
	//Return If Not Valid
	if (!bIsInitialized || ScreenplayUserWidgetClass == nullptr)
		return;

	//Create Screenplay Widget
	UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget = CreateWidget<UFluidNarratorScreenplayUserWidget>(GetWorld(), ScreenplayUserWidgetClass);
	ScreenplayUserWidget->AddToViewport();
	ScreenplayUserWidget_Out = ScreenplayUserWidget;

	//Get Culture (Use Default If Input Is Empty)
	CultureOverride = CultureOverride == "" ? FName(LocalizationCulture.Language) : CultureOverride;
	
	//Start
	NarratorScreenplaySystem.StartBySectionAuto(CultureOverride,TextAsString,ScreenplayUserWidget,nullptr,ScreenplayTask_Out);

	//Task Not Started So Delete User Widget
	if (ScreenplayTask_Out == -1)
		ScreenplayUserWidget->RemoveFromParent();
}

void UFluidNarratorSubsystem::StartScreenplayTaskBySectionNameByScreenplayAsset(FName CultureOverride, UFluidNarratorScreenplay* Screenplay, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, const FName SectionName,  UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out)
{
	//Return If Not Valid
	if (!bIsInitialized || ScreenplayUserWidgetClass == nullptr || Screenplay == nullptr)
		return;

	//Create Screenplay Widget
	UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget = CreateWidget<UFluidNarratorScreenplayUserWidget>(GetWorld(),ScreenplayUserWidgetClass);
	ScreenplayUserWidget->AddToViewport();
	ScreenplayUserWidget_Out = ScreenplayUserWidget;

	//Get Culture (Use Default If Input Is Empty)
	CultureOverride = CultureOverride == "" ? FName(LocalizationCulture.Language) : CultureOverride;
	
	//Start
	NarratorScreenplaySystem.StartBySectionName(CultureOverride,Screenplay->Text,SectionName,ScreenplayUserWidget,nullptr,ScreenplayTask_Out);

	//Task Not Started So Delete User Widget
	if (ScreenplayTask_Out == -1)
		ScreenplayUserWidget->RemoveFromParent();
}

void UFluidNarratorSubsystem::StartScreenplayTaskBySectionAutoByScreenplayAsset(FName CultureOverrideName, UFluidNarratorScreenplay* Screenplay, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out)
{
	//Return If Not Valid
	if (!bIsInitialized || ScreenplayUserWidgetClass == nullptr || Screenplay == nullptr)
		return;

	//Create Screenplay Widget
	UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget = CreateWidget<UFluidNarratorScreenplayUserWidget>(GetWorld(),ScreenplayUserWidgetClass);
	ScreenplayUserWidget->AddToViewport();
	ScreenplayUserWidget_Out = ScreenplayUserWidget;

	//Get Culture (Use Default If Input Is Empty)
	CultureOverrideName = CultureOverrideName == "" ? FName(LocalizationCulture.Language) : CultureOverrideName;
	
	//Start
	NarratorScreenplaySystem.StartBySectionAuto(CultureOverrideName,Screenplay->Text,ScreenplayUserWidget,nullptr,ScreenplayTask_Out);

	//Task Not Started So Delete User Widget
	if (ScreenplayTask_Out == -1)
		ScreenplayUserWidget->RemoveFromParent();
}

void UFluidNarratorSubsystem::GetScreenplayTaskCount(int& TaskCount_Out)
{
	//Return If Not Valid
	if (!bIsInitialized)
		return;

	//Get Task Count
	NarratorScreenplaySystem.GetTaskCount(TaskCount_Out);
}

/*=============================================================================
Fluid Narrator Subsystem : Localization
=============================================================================*/

void UFluidNarratorSubsystem::SetLocalizationAsset(UFluidNarratorLocalization* FluidNarratorLocalizationAsset)
{
	NarratorScreenplaySystem.Localization = FluidNarratorLocalizationAsset;
	LocalizationAsset = FluidNarratorLocalizationAsset;

	//Cultures To Array
	TArray<FFluidNarratorLocalizationCulture> CultureArray;
	LocalizationAsset->Cultures.GenerateValueArray(CultureArray);

	//Get Culture
	for (int i = 0; i < CultureArray.Num(); i++)
		if (CultureArray[i].bIsNativeCulture || i == 0)
			LocalizationCulture = CultureArray[i];
}

void UFluidNarratorSubsystem::SetLocalizationCulture(FName CultureName)
{
	//Not Valid
	if (LocalizationAsset == nullptr)
		return;

	//Set Culture
	if (LocalizationAsset->Cultures.Contains(CultureName))
		LocalizationCulture = LocalizationAsset->Cultures[CultureName];
}

void UFluidNarratorSubsystem::GetLocalizationText(FText Text, FText& Text_Out)
{
	//Not Valid
	if (LocalizationAsset == nullptr)
	    return;

	//Localize
	LocalizationAsset->GetLocalizationTextAsText(FName(LocalizationCulture.Language), Text,Text_Out);
	
	//Reapply Variables
	FString TextAsString = Text_Out.ToString();
	FluidNarratorScreenplaySystem::ParseVariablesApplyToString(TextAsString,NarratorScreenplaySystem.Variables,TextAsString);
	Text_Out = FText::FromString(TextAsString);
}

void UFluidNarratorSubsystem::GetLocalizationTexts(TArray<FText> Texts, TArray<FText>& Texts_Out)
{
	//Not Valid
	if (LocalizationAsset == nullptr)
		return;

	//Localize
	for (int i = 0; i < Texts.Num(); i++)
		GetLocalizationText(Texts[i],Texts_Out[i]);
}

void UFluidNarratorSubsystem::GetLocalizationName(const FName Name, FName& Name_Out)
{
	if (LocalizationAsset == nullptr)
		return;

	//Localize
	LocalizationAsset->GetLocalizationTextAsName(FName(LocalizationCulture.Language),Name,Name_Out);

	//Reapply Variables
	FString TextAsString = Name_Out.ToString();
	FluidNarratorScreenplaySystem::ParseVariablesApplyToString(TextAsString,NarratorScreenplaySystem.Variables,TextAsString);
	Name_Out = FName(TextAsString);
}

void UFluidNarratorSubsystem::GetLocalizationNames(const TArray<FName> Names, TArray<FName>& Names_Out)
{
	//Not Valid
	if (LocalizationAsset == nullptr)
		return;

	//Localize
	for (int i = 0; i < Names.Num(); i++)
		GetLocalizationName(Names[i],Names_Out[i]);
}

/*=============================================================================
Fluid Narrator Subsystem : Translate
=============================================================================*/

void UFluidNarratorSubsystem::CreateTranslator(UFluidNarratorTranslator*& Translator_Out)
{
	Translator_Out = NewObject<UFluidNarratorTranslator>();
}