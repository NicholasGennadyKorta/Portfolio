// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidArchitectHexaSphereEditor.h"

#define LOCTEXT_NAMESPACE "FFluidArchitectHexaSphereEditorModule"

void FFluidArchitectHexaSphereEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type Catagory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Fluid")), FText::FromString("Fluid"));
	TSharedPtr<IAssetTypeActions> Asset = MakeShareable(new FluidArchitectHexaSphereEditorAssetTypeActions(Catagory));
	AssetTools.RegisterAssetTypeActions(Asset.ToSharedRef());
	CreatedAssetTypeActions.Add(Asset);
}

void FFluidArchitectHexaSphereEditorModule::ShutdownModule()
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

IMPLEMENT_MODULE(FFluidArchitectHexaSphereEditorModule, FluidArchitectHexaSphereEditor)