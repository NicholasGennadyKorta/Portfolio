// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidArchitectCoreEditor.h"
#define LOCTEXT_NAMESPACE "FFluidArchitectCoreEditorModule"

void FFluidArchitectCoreEditorModule::StartupModule()
{
    {
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        EAssetTypeCategories::Type Catagory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Fluid")), FText::FromString("Fluid"));
    }
}

void FFluidArchitectCoreEditorModule::ShutdownModule()
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
	
IMPLEMENT_MODULE(FFluidArchitectCoreEditorModule, FluidArchitectCoreEditor)