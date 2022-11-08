// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidArchitectHexaSphereEditorAsset.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect HexaSphere Editor Asset Factory

 UFluidArchitectHexaSphereEditorAssetFactory::UFluidArchitectHexaSphereEditorAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidArchitectHexaSphereProfile::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject*  UFluidArchitectHexaSphereEditorAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidArchitectHexaSphereProfile* NewObjectAsset = NewObject<UFluidArchitectHexaSphereProfile>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect HexaSphere Editor Asset Type Actions

FluidArchitectHexaSphereEditorAssetTypeActions::FluidArchitectHexaSphereEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FluidArchitectHexaSphereEditorAssetTypeActions::GetName() const
{
    return FText::FromString("Fluid Architect HexaSphere Profile");
}

FColor FluidArchitectHexaSphereEditorAssetTypeActions::GetTypeColor() const
{
    return FColor(0, 0, 0);
}

UClass* FluidArchitectHexaSphereEditorAssetTypeActions::GetSupportedClass() const
{
    return UFluidArchitectHexaSphereProfile::StaticClass();
}

uint32 FluidArchitectHexaSphereEditorAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}

void FluidArchitectHexaSphereEditorAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (UFluidArchitectHexaSphereProfile* Asset = Cast<UFluidArchitectHexaSphereProfile>(*ObjIt))
        {

            TSharedRef<FluidArchitectCoreEditorProfile> NewGraphEditor(new FluidArchitectCoreEditorProfile());
            NewGraphEditor->Initialize(Mode, EditWithinLevelEditor, Asset);
        }
    }
}