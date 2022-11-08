// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"
#include "AssetToolsModule.h"
#include "FluidAIUtilityPlannerEditorAssets.h"
#include "FluidAIUtilityPlannerEditorAgentEditor.h"
#include "FluidAIUtilityPlannerEditorAgentEdGraph.h"
#include "FluidAIUtilityPlannerEditorAssets.h"

class FFluidAIUtilityPlannerEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
