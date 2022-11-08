// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FluidAIUtilityPlannerSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "FluidAIUtilityPlannerAdditions.generated.h"

//Fluid AI Utility Planner Additions Blueprint Function Library
UCLASS()
class FLUIDAIUTILITYPLANNERRUNTIME_API UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="Fluid AI Utility Planner Additions")
		static void GetFluidAIUtilityPlannerMaxAgentNumb(int& Numb_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Fluid AI Utility Planner Additions")
		static UFluidAIUtilityPlannerSubsystem* GetFluidAIUtilityPlannerSubsystem();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Fluid AI Utility Planner Additions")
		static void GetWorldRandomReachablePointInRadius(UWorld* World, const FVector Origin, const float Radius, FVector& Location_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Fluid AI Utility Planner Additions")
		static void GetWorldRandomReachablePointInRadiusUnsafe(UWorld* World, const FVector Origin, const float Radius, FVector& Location_Out);
	UFUNCTION(BlueprintCallable, Category="Fluid AI Utility Planner Additions")
		static void PawnMoveDirection(APawn* Pawn, const FVector Direction, const float Scale, const bool bForce);
	UFUNCTION(BlueprintCallable, Category="Fluid AI Utility Planner Additions")
		static void PawnMoveDirectionUnsafe(APawn* Pawn, const FVector Direction, const float Scale, const bool bForce);
	UFUNCTION(BlueprintCallable, Category="Fluid AI Utility Planner Additions")
		static void PawnMoveToLocation(APawn* Pawn, const FVector Location, const float Scale, const bool bForce);
	UFUNCTION(BlueprintCallable, Category="Fluid AI Utility Planner Additions")
		static void PawnMoveToLocationUnsafe(APawn* Pawn, const FVector Location, const float Scale, const bool bForce);
};