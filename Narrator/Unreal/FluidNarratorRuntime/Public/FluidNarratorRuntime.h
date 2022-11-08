// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidNarratorTranslator.h"
#include "FluidNarratorDeveloperSettings.h"
#include "FluidNarratorData.h"
#include "FluidNarratorScreenplaySystem.h"
#include "FluidNarratorScreenplaySlateWidget.h"
#include "FluidNarratorScreenplayUserWidget.h"
#include "FluidNarratorSubsystem.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * 
 */

class FFluidNarratorRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
