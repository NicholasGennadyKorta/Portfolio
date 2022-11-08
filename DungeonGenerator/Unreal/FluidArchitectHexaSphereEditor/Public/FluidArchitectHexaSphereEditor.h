// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidArchitectHexaSphereEditorAsset.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"
#include "AssetToolsModule.h"

class FFluidArchitectHexaSphereEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
