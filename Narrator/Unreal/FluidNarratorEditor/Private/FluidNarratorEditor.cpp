// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidNarratorEditor.h"
#include "FluidNarratorEditorAssets.h"

#define LOCTEXT_NAMESPACE "FFluidNarratorEditorModule"

void FFluidNarratorEditorModule::StartupModule()
{
    {
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        const EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Fluid")), FText::FromString("Fluid"));
        const TSharedPtr<IAssetTypeActions> NarratorScreenPlayAssetTypeActions = MakeShareable(new FluidNarratorScreenplayEditorAssetTypeActions(Category));
        const TSharedPtr<IAssetTypeActions> NarratorLocalizationAssetTypeActions = MakeShareable(new FluidNarratorLocalizationEditorAssetTypeActions(Category));
        AssetTools.RegisterAssetTypeActions(NarratorScreenPlayAssetTypeActions.ToSharedRef());
        AssetTools.RegisterAssetTypeActions(NarratorLocalizationAssetTypeActions.ToSharedRef());
        CreatedAssetTypeActions.Add(NarratorScreenPlayAssetTypeActions);
        CreatedAssetTypeActions.Add(NarratorLocalizationAssetTypeActions);
    }
}

void FFluidNarratorEditorModule::ShutdownModule()
{ 
    // Unregister all the asset types that we registered
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
            AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
    }
    CreatedAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFluidNarratorEditorModule, FluidNarratorEditor)
