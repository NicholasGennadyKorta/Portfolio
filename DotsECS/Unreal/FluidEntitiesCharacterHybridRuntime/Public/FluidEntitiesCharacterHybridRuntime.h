// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidEntitiesCoreRuntime.h"
#include "FluidEntitiesCharacterHybridData.h"
#include "FluidEntitiesCharacterHybridBlueprintLibrary.h"
#include "FluidEntitiesCharacterHybridAnim.h"
#include "FluidEntitiesCharacterHybridAnimNodes.h"
#include "FluidEntitiesCharacterHybridSubsystem.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFluidEntitiesCharacterHybridRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
