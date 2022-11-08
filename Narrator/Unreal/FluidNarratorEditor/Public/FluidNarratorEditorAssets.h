// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidNarratorSubsystem.h"
#include "FluidNarratorEditor.h"
#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"
#include "FluidNarratorEditorAssets.generated.h"

/**
 * 
 */

//Fluid Narrator Screenplay Asset Factory
UCLASS()
class UFluidNarratorScreenplayEditorAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//Fluid Narrator Localization Editor Asset Factory
UCLASS()
class UFluidFluidNarratorLocalizationEditorAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//Fluid Narrator Screenplay Asset Type Actions
class FluidNarratorScreenplayEditorAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FluidNarratorScreenplayEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
   
private:
	EAssetTypeCategories::Type MyAssetCategory;
};

//Fluid Narrator Localization Editor Asset Type Actions
class FluidNarratorLocalizationEditorAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FluidNarratorLocalizationEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

private:
	EAssetTypeCategories::Type MyAssetCategory;
};