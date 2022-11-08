// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidAIUtilityPlannerEditor.h"

#define LOCTEXT_NAMESPACE "FFluidAIUtilityPlannerEditorModule"

void FFluidAIUtilityPlannerEditorModule::StartupModule()
{
	//Asset Tool || Category
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	const EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Fluid")), FText::FromString("Fluid"));
	
	//Agent Asset
	const TSharedPtr<IAssetTypeActions> AgentAsset = MakeShareable(new FluidAIUtilityPlannerAgentEditorAssetTypeActions(Category));
	AssetTools.RegisterAssetTypeActions(AgentAsset.ToSharedRef());
	CreatedAssetTypeActions.Add(AgentAsset);
}

void FFluidAIUtilityPlannerEditorModule::ShutdownModule()
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

IMPLEMENT_MODULE(FFluidAIUtilityPlannerEditorModule,FluidAIUtilityPlannerEditor)