// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidNarratorScreenplayEditorMarshaller.h"
#include "FluidNarratorLocalizationCulturesSlateWidget.h"
#include "FluidNarratorLocalizationTextsSlateWidget.h"
#include "FluidNarratorScreenplayScriptViewSlateWidget.h"
#include "FluidNarratorScreenplaySimulatorSlateWidget.h"
#include "FluidNarratorEditorAssets.h"
#include "FluidNarratorLocalizationEditor.h"
#include "FluidNarratorScreenplayEditor.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"
#include "AssetToolsModule.h"

/**
 * 
 */

class FFluidNarratorEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
