// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidAstroStarMap.h"

/*=============================================================================
Fluid Astro Star Map Model HyqV3
=============================================================================*/

void UFluidAstroStarMapModelHyqV3::LoadData()
{
	//Start With New Set Of Empty Stars
	StarNames.Empty();
	StarPositions.Empty();
	StarMagnitudes.Empty();
	StarCount = 0;
	
	//Return If Not Valid
	if (StarMapModelHyqV3DataTable == nullptr)
		return;
	
	//Add Stars
	for(const auto Row : StarMapModelHyqV3DataTable->GetRowMap())
	{
		//Get Row Data
		FFluidAstroStarMapModelHyqV3TableRow* Data = (FFluidAstroStarMapModelHyqV3TableRow*)(Row.Value);

		//Only If Visible To Human Eye
		if (Data->Mag <= 6.5f)
		{
			StarNames.Add(FText::FromString(Data->Proper));
			StarDistancesInParsecs.Add(Data->Dist);
			StarDistancesInLightYears.Add(Data->Dist *  3.262f);
			StarMagnitudes.Add(Data->Mag);
			StarPositions.Add(FVector(Data->X,Data->Y,Data->Z) * 15000);
			StarCount++;
		}
	}
}

FVector UFluidAstroStarMapModelHyqV3::LatitudeLongitudeToPoint(const float Latitude, const float Longitude, const float Radius) const
{
	// Calculate point On Sphere From Longitude / Latitude (In Radians), And Radius Of The Point
	const float R = FMath::Cos(Latitude);
	const float Y = FMath::Sin(Latitude);
	const float X = FMath::Sin(Longitude) * R;
	const float Z = -FMath::Cos(Longitude) * R;
	return FVector(X, Y, Z) * Radius;
}

/*=============================================================================
Fluid Astro Star Map Actor
=============================================================================*/

AFluidAstroStarMapActor::AFluidAstroStarMapActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AFluidAstroStarMapActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
}

void AFluidAstroStarMapActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFluidAstroStarMapActor::DrawDebugStars()
{
	//Calculate Base Data
	const FVector BaseExtent = FVector::One() * StarRenderSize * 100000;
	const float BasePositionSize = StarDistanceRenderSize;

	//Draw Debug
	for (int i = 0; i < StarMapModel->StarCount; i ++)
	{
		DrawDebugBox(GetWorld(),StarMapModel->StarPositions[i] * BasePositionSize, BaseExtent, FColor::White);
	}
}

void AFluidAstroStarMapActor::GetStarRenderPositions(TArray<FVector>& RenderLocations_Out)
{
	//Empty Render Locations (Make Sure If Passed In By C++ To Empty It)
	RenderLocations_Out.Empty();
	
	//Calculate Base Data
	const float BasePositionSize = StarDistanceRenderSize;

	//Draw Debug
	for (int i = 0; i < StarMapModel->StarCount; i ++)
		RenderLocations_Out.Add(StarMapModel->StarPositions[i] * BasePositionSize);
}



