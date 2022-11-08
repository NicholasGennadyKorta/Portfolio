// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DrawDebugHelpers.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "FluidArchitectCoreSubsystem.h"
#include "FluidArchitectHexaSphereProfile.generated.h"

UCLASS(BlueprintType)
class FLUIDARCHITECTHEXASPHERERUNTIME_API UFluidArchitectHexaSphereProfile: public UFluidArchitectCoreProfile
{
	GENERATED_BODY()

	//Main	
	public:
	
	void Initialize() override;
	void Generate() override;
	void OnPreviewViewport(FName ModuleID, UWorld* World) override;
};