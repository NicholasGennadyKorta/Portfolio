// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FluidAIUtilityPlannerSubsystem.h"
#include "FluidAIUtilityPlannerAdditions.h"
#include "Modules/ModuleManager.h"

class FFluidAIUtilityPlannerRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
