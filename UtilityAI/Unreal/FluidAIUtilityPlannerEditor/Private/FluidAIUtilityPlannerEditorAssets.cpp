// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAIUtilityPlannerEditorAssets.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Agent Editor Asset Factory

UFluidAIUtilityPlannerAgentEditorAssetFactory::UFluidAIUtilityPlannerAgentEditorAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidAIUtilityPlannerAgent::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidAIUtilityPlannerAgentEditorAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidAIUtilityPlannerAgent* NewObjectAsset = NewObject<UFluidAIUtilityPlannerAgent>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Agent Editor Asset Type Actions

FluidAIUtilityPlannerAgentEditorAssetTypeActions::FluidAIUtilityPlannerAgentEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FluidAIUtilityPlannerAgentEditorAssetTypeActions::GetName() const
{
    return FText::FromString("Fluid AI Utility Planner Agent");
}

FColor FluidAIUtilityPlannerAgentEditorAssetTypeActions::GetTypeColor() const
{
    return FColor(0, 0, 0);
}

UClass* FluidAIUtilityPlannerAgentEditorAssetTypeActions::GetSupportedClass() const
{
    return UFluidAIUtilityPlannerAgent::StaticClass();
}

uint32 FluidAIUtilityPlannerAgentEditorAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}

void FluidAIUtilityPlannerAgentEditorAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (UFluidAIUtilityPlannerAgent* Asset = Cast<UFluidAIUtilityPlannerAgent>(*ObjIt))
        {
            TSharedRef<FluidAIUtilityPlannerEditorAgentEditor> NewGraphEditor(new FluidAIUtilityPlannerEditorAgentEditor());
            NewGraphEditor->Initialize(Mode, EditWithinLevelEditor, Asset);
        }
    }
}