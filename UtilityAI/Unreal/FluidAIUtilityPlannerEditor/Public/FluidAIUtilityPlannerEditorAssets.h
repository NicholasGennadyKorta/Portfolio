// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"
#include "FluidAIUtilityPlannerSubsystem.h"
#include "FluidAIUtilityPlannerEditor.h"
#include "FluidAIUtilityPlannerEditorAssets.generated.h"

//Fluid AI Utility Planner Agent Editor Asset Factory
UCLASS()
class UFluidAIUtilityPlannerAgentEditorAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//Fluid AI Utility Planner Agent Editor Asset Type Actions
class FluidAIUtilityPlannerAgentEditorAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FluidAIUtilityPlannerAgentEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
   
private:
	EAssetTypeCategories::Type MyAssetCategory;
};


