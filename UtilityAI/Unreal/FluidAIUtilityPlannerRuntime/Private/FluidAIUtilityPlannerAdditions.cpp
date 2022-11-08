// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidAIUtilityPlannerAdditions.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Additions Blueprint Function Library

void UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::GetFluidAIUtilityPlannerMaxAgentNumb(int& Numb_Out)
{
	Numb_Out = FLUID_AI_UTILITY_PLANNER_AGENTS_MAX;
}

UFluidAIUtilityPlannerSubsystem* UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::GetFluidAIUtilityPlannerSubsystem()
{
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->GetWorld() && GEngine->GameViewport->GetWorld()->GetGameInstance())
		return GEngine->GameViewport->GetWorld()->GetGameInstance()->GetSubsystem<UFluidAIUtilityPlannerSubsystem>();
	return  nullptr;
}

void UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::GetWorldRandomReachablePointInRadius(UWorld* World, const FVector Origin, const float Radius, FVector& Location_Out)
{
	if (World)
		Location_Out = FVector::Zero();
}

void UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::GetWorldRandomReachablePointInRadiusUnsafe(UWorld* World, const FVector Origin, const float Radius, FVector& Location_Out)
{
	Location_Out = FVector::Zero();
}

void UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::PawnMoveDirection(APawn* Pawn, const FVector Direction, const float Scale, const bool bForce)
{
	//Add Movement Input
	if (Pawn)
		Pawn->AddMovementInput(Direction.GetSafeNormal(),Scale, bForce);
}

void UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::PawnMoveDirectionUnsafe(APawn* Pawn, const FVector Direction, const float Scale, const bool bForce)
{
	//Add Movement Input
	Pawn->AddMovementInput(Direction.GetSafeNormal(),Scale, bForce);
}

void UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::PawnMoveToLocation(APawn* Pawn, const FVector Location, const float Scale, const bool bForce)
{
	//Convert Point To Direction And Move
	if (Pawn)
		PawnMoveDirection(Pawn,(Location - Pawn->GetActorLocation()).GetSafeNormal(),Scale, bForce);
}

void UFluidAIUtilityPlannerAdditionsBlueprintFunctionLibrary::PawnMoveToLocationUnsafe(APawn* Pawn, const FVector Location, const float Scale, const bool bForce)
{
	//Convert Point To Direction And Move
	PawnMoveDirectionUnsafe(Pawn,(Location - Pawn->GetActorLocation()).GetSafeNormal(),Scale, bForce);
}