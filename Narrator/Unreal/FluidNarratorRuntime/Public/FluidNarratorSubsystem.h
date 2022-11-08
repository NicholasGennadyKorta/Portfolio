// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorRuntime.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Internationalization/Text.h"
#include "UObject/Object.h"
#include "FluidNarratorSubsystem.generated.h"

/**
 * 
 */

//Fluid Entities Character Hybrid Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid Narrator"), STATGROUP_FluidNarrator, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid Narrator Screenplay"),STAT_FluidNarrator_Screenplay,STATGROUP_FluidNarrator);

class FluidNarratorScreenplaySystem;

//Fluid Narrator Subsystem
UCLASS(BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Narrator Subsystem : Main
public:	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	
	bool bIsInitialized;

//Fluid Narrator Subsystem : Screenplay
public:		

	UFUNCTION(BlueprintCallable)
		void StartScreenplayTaskBySectionNameByText(FName CultureOverride, const FString TextAsString, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, const FName SectionName, UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out);
	UFUNCTION(BlueprintCallable)
		void StartScreenplayTaskBySectionAutoByText(FName CultureOverride, const FString TextAsString, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out);
	UFUNCTION(BlueprintCallable)
		void StartScreenplayTaskBySectionNameByScreenplayAsset(FName CultureOverride, UFluidNarratorScreenplay* Screenplay, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, const FName SectionName, UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out);
	UFUNCTION(BlueprintCallable)
		void StartScreenplayTaskBySectionAutoByScreenplayAsset(FName CultureOverrideName, UFluidNarratorScreenplay* Screenplay, TSubclassOf<UFluidNarratorScreenplayUserWidget> ScreenplayUserWidgetClass, UFluidNarratorScreenplayUserWidget*& ScreenplayUserWidget_Out, int& ScreenplayTask_Out);
	UFUNCTION(BlueprintCallable)
		void GetScreenplayTaskCount(int& TaskCount_Out);
	
	FluidNarratorScreenplaySystem NarratorScreenplaySystem;

//Fluid Narrator Subsystem : Localization
public:		

	UFUNCTION(BlueprintCallable)
		void SetLocalizationAsset(UFluidNarratorLocalization* FluidNarratorLocalizationAsset);
	UFUNCTION(BlueprintCallable)
		void SetLocalizationCulture(FName Culture);
	UFUNCTION(BlueprintCallable)
		void GetLocalizationText(FText Text, FText& Text_Out);
	UFUNCTION(BlueprintCallable)
		void GetLocalizationTexts(TArray<FText> Texts, TArray<FText>& Texts_Out);
	UFUNCTION(BlueprintCallable)
		void GetLocalizationName(const FName Name, FName& Name_Out);
	UFUNCTION(BlueprintCallable)
		void GetLocalizationNames(const TArray<FName> Names, TArray<FName>& Names_Out);
	
	FFluidNarratorLocalizationCulture LocalizationCulture;
	UFluidNarratorLocalization* LocalizationAsset;


//Fluid Narrator Subsystem : Translation
public:
	
	UFUNCTION(BlueprintCallable)
		void CreateTranslator(UFluidNarratorTranslator*& Translator_Out);
};
