// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidNarratorEditorAssets.h"

/*=============================================================================
Fluid Narrator Screenplay Asset Factory
=============================================================================*/

UFluidNarratorScreenplayEditorAssetFactory::UFluidNarratorScreenplayEditorAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass =  UFluidNarratorScreenplay::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidNarratorScreenplayEditorAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
     UFluidNarratorScreenplay* NewObjectAsset = NewObject< UFluidNarratorScreenplay>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

/*=============================================================================
Fluid Narrator Localization Asset Factory
=============================================================================*/

UFluidFluidNarratorLocalizationEditorAssetFactory::UFluidFluidNarratorLocalizationEditorAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass =  UFluidNarratorLocalization::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidFluidNarratorLocalizationEditorAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidNarratorLocalization* NewObjectAsset = NewObject< UFluidNarratorLocalization>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

/*=============================================================================
Fluid Narrator Screenplay Asset Type Actions
=============================================================================*/

FluidNarratorScreenplayEditorAssetTypeActions::FluidNarratorScreenplayEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{
}

FText FluidNarratorScreenplayEditorAssetTypeActions::GetName() const
{
    return FText::FromString("Fluid Narrator Screenplay");
}

FColor FluidNarratorScreenplayEditorAssetTypeActions::GetTypeColor() const
{
    return FColor(0, 0, 0);
}

UClass* FluidNarratorScreenplayEditorAssetTypeActions::GetSupportedClass() const
{
    return   UFluidNarratorScreenplay::StaticClass();
}

uint32 FluidNarratorScreenplayEditorAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}

void FluidNarratorScreenplayEditorAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (UFluidNarratorScreenplay* Asset = Cast<UFluidNarratorScreenplay>(*ObjIt))
        {
            TSharedRef<FluidNarratorScreenplayEditor> NewGraphEditor(new FluidNarratorScreenplayEditor());
            NewGraphEditor->Initialize(Mode, EditWithinLevelEditor, Asset);
        }
    }
}

/*=============================================================================
Fluid Narrator Localization Asset Type Actions
=============================================================================*/

FluidNarratorLocalizationEditorAssetTypeActions::FluidNarratorLocalizationEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{
}

FText FluidNarratorLocalizationEditorAssetTypeActions::GetName() const
{
    return FText::FromString("Fluid Narrator Localization");
}

FColor FluidNarratorLocalizationEditorAssetTypeActions::GetTypeColor() const
{
    return FColor(0, 0, 0);
}

UClass* FluidNarratorLocalizationEditorAssetTypeActions::GetSupportedClass() const
{
    return  UFluidNarratorLocalization::StaticClass();
}

uint32 FluidNarratorLocalizationEditorAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}

void FluidNarratorLocalizationEditorAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (UFluidNarratorLocalization* Asset = Cast<UFluidNarratorLocalization>(*ObjIt))
        {
            TSharedRef<FluidNarratorLocalizationEditor> NewGraphEditor(new FluidNarratorLocalizationEditor());
            NewGraphEditor->Initialize(Mode, EditWithinLevelEditor, Asset);
        }
    }
}