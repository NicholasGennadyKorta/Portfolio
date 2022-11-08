// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidAstroNewtonBodies.h"
#include "FluidAstroStarMap.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFluidAstroRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
