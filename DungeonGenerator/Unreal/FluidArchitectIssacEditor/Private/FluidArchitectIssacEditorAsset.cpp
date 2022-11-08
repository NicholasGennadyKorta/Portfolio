// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidArchitectIssacEditorAsset.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Editor Asset Factory

UFluidArchitectIssacEditorAssetFactory::UFluidArchitectIssacEditorAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidArchitectIssacProfile::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidArchitectIssacEditorAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidArchitectIssacProfile* NewObjectAsset = NewObject<UFluidArchitectIssacProfile>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Editor Asset Type Actions

FluidArchitectIssacEditorAssetTypeActions::FluidArchitectIssacEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FluidArchitectIssacEditorAssetTypeActions::GetName() const
{
    return FText::FromString("Fluid Architect Issac Profile");
}

FColor FluidArchitectIssacEditorAssetTypeActions::GetTypeColor() const
{
    return FColor(0, 0, 0);
}

UClass* FluidArchitectIssacEditorAssetTypeActions::GetSupportedClass() const
{
    return  UFluidArchitectIssacProfile::StaticClass();
}

uint32 FluidArchitectIssacEditorAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}

void FluidArchitectIssacEditorAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (UFluidArchitectIssacProfile* Asset = Cast<UFluidArchitectIssacProfile>(*ObjIt))
        {

            TSharedRef<FluidArchitectCoreEditorProfile> NewGraphEditor(new FluidArchitectCoreEditorProfile());
            NewGraphEditor->Initialize(Mode, EditWithinLevelEditor, Asset);
        }
    }
}