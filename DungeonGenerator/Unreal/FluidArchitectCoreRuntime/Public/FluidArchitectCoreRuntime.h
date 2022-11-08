// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidArchitectCoreSubsystem.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFluidArchitectCoreRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
