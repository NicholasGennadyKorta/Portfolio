// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAstroKeplerianBodies.h"

/*=============================================================================
Fluid Astro KeplerianBodies Subsystem : Debug
=============================================================================*/

static TAutoConsoleVariable<int32> CDebugFluidAstroKeplerianBodiesDebugDraw(
	TEXT("Fluid.Astro.Keplerian.Bodies.Debug.Draw"),
	0,
	TEXT("Fluid.Astro.Keplerian.Bodies.Debug.Draw\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);

/*=============================================================================
Fluid Astro KeplerianBodies Subsystem : Initialize | Tick
=============================================================================*/

void UFluidAstroKeplerianBodiesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//Initialize Once
	if (bInitialized) return;
	bInitialized = true;

	//Initialize
	InitializeKeplerianBodies();
	SimulateInitialize();                      
}

void UFluidAstroKeplerianBodiesSubsystem::Deinitialize()
{
	bInitialized = false;

	//for (int i = 0; i < FLUID_ASTRO_CELESTIALS_ENTITIES_MAX; i++)
	//	DestroyEntity(i);
}

void UFluidAstroKeplerianBodiesSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;

	//Simulate
	SimulateTick(DeltaTime);

	//Debug Draw
	DebugDraw();
}

bool UFluidAstroKeplerianBodiesSubsystem::IsTickable() const
{
	return !IsTemplate() && bInitialized;
}

bool UFluidAstroKeplerianBodiesSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidAstroKeplerianBodiesSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidAstroKeplerianBodiesSubsystem::GetStatId() const
{
	return TStatId();
}

/*=============================================================================
Fluid Astro KeplerianBodies Subsystem : Entity Management
=============================================================================*/

void UFluidAstroKeplerianBodiesSubsystem::InitializeKeplerianBodies()
{
	// Initialize Entity Available Queue With All Possible Entities
	for (int i = 0; i < FLUID_ASTRO_KEPLERIAN_BODIES_MAX; ++i)
		KeplerianBodiesAvailable.push(i);
}

void UFluidAstroKeplerianBodiesSubsystem::AddKeplerianBodyRaw(const FName Name, const FName Id, const FName ParentId, const FColor Color, const double OrbitAphelion, const double OrbitPerihelion, const double OrbitInclination, const double OrbitPeriod, const double OrbitCenterX, const double OrbitCenterY, const double BodyRadius, int& KeplerianBody_Out)
{
	//Return If We Reached Max Entity Count
	if (KeplerianBodiesCount >= FLUID_ASTRO_KEPLERIAN_BODIES_MAX)
	{
		KeplerianBody_Out = -1;
		return;
	}
    
	// Take A Entity From The Front Of Available Queue
	KeplerianBody_Out = KeplerianBodiesAvailable.front();
	KeplerianBodiesAvailable.pop();
	++KeplerianBodiesCount;
	
	//Add Entity Data
	KeplerianBodiesNames[KeplerianBody_Out] = Name;
	KeplerianBodiesIds[KeplerianBody_Out] = Id;
	KeplerianBodiesParentIds[KeplerianBody_Out] = ParentId;
	KeplerianBodiesColors[KeplerianBody_Out] = Color;
	KeplerianBodiesOrbitOrbitAphelions[KeplerianBody_Out] = OrbitAphelion;
	KeplerianBodiesOrbitOrbitPerihelions[KeplerianBody_Out] = OrbitPerihelion;
	KeplerianBodiesOrbitInclinations[KeplerianBody_Out] = OrbitInclination;
	KeplerianBodiesOrbitPeriods[KeplerianBody_Out] = OrbitPeriod;
	KeplerianBodiesBodyRadiuses[KeplerianBody_Out] = BodyRadius;
	bIsKeplerianBodiesActives[KeplerianBody_Out] = true;
}

void UFluidAstroKeplerianBodiesSubsystem::AddKeplerianBodiesByDataTable(UDataTable* DataTable, TArray<int>& KeplerianBodies_Out)
{
	//Return If Not Valid
	if ( DataTable == nullptr)
		return;

	//Add Entities
	AddKeplerianBodiesByDataTableRows(DataTable,DataTable->GetRowNames(),KeplerianBodies_Out);
}

void UFluidAstroKeplerianBodiesSubsystem::AddKeplerianBodiesByDataTableRow(UDataTable* DataTable, const FName RowName, int& KeplerianBodies_Out)
{
	//Get Row
	const FFluidAstroKeplerianBodiesTableRow* Row = DataTable->FindRow<FFluidAstroKeplerianBodiesTableRow>(RowName,TEXT(""));
	if (!Row)
	{
		KeplerianBodies_Out = -1;
		return;
	}
}

void UFluidAstroKeplerianBodiesSubsystem::AddKeplerianBodiesByDataTableRows(UDataTable* DataTable, const TArray<FName> RowNames, TArray<int>& KeplerianBodies_Out)
{
	//Add Entities
	for (int i = 0; i < RowNames.Num(); i++)
	{
		int Entity = -1;
		AddKeplerianBodiesByDataTableRow(DataTable,RowNames[i],Entity);
		if (Entity != -1) KeplerianBodies_Out.Add(Entity);
	}
}

void UFluidAstroKeplerianBodiesSubsystem::DestroyKeplerianBody(const int KeplerianBody)
{
	//Return If Entity Not Valid
	if (KeplerianBodiesCount >= FLUID_ASTRO_KEPLERIAN_BODIES_MAX)
		return;
	
	//Destroy Entity
	KeplerianBodiesAvailable.push(KeplerianBody);
	--KeplerianBodiesCount;
	
	//Set Entity Not Active
	bIsKeplerianBodiesActives[KeplerianBody] = false;
}

/*=============================================================================
Fluid Astro KeplerianBodies Subsystem : Simulations
=============================================================================*/

void UFluidAstroKeplerianBodiesSubsystem::SimulateInitialize()
{
	//Simulation
}

void UFluidAstroKeplerianBodiesSubsystem::SimulateTick(const float DeltaTime)
{
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidAstroKeplerianBodies_UpdateOrbitInputs);
	{
		//Update Orbit Data
		if (bIsKeplerianBodiesDirty)
		{
			const int OrbitLineSegments = 50;
			
			for (int i = 0; i < FLUID_ASTRO_KEPLERIAN_BODIES_MAX; i++)
			{
				// Skip If Not Active
				if (!bIsKeplerianBodiesActives[i])
					continue;

				//Get Orbit Data
				const double OrbitCenterX = KeplerianBodiesOrbitCenterXs[i];
				const double OrbitCenterY = KeplerianBodiesOrbitCenterYs[i];
				const double OrbitAphelion = KeplerianBodiesOrbitOrbitPerihelions[i];
				const double OrbitPerihelion = KeplerianBodiesOrbitOrbitPerihelions[i];
				const double OrbitInclination = KeplerianBodiesOrbitInclinations[i];
				
				//Calculate Orbit Data Calculated
				const double OrbitSemiMajorLength = KeplerianBodiesOrbitSemiMajorLengths[i] = (OrbitPerihelion + OrbitAphelion) / 2; //Cache The Value So We Do Not Need To Reacces Array Below
				const double OrbitLinearEccentricity  = OrbitSemiMajorLength - OrbitPerihelion;
				const double OrbitSemiMinorLength = KeplerianBodiesOrbitSemiMinorLengths[i] = FMath::Sqrt(FMath::Pow(OrbitSemiMajorLength, 2) - FMath::Pow(OrbitLinearEccentricity, 2));
				KeplerianBodiesOrbitCenterXs[i] = OrbitCenterX - OrbitLinearEccentricity;
				KeplerianBodiesOrbitCenterYs[i] = OrbitCenterY;

				//Get Orbit Positions
				KeplerianBodiesOrbitPositionss[i].Empty();
				for (int j = 0; j < OrbitLineSegments; j++)
				{
					//Get Angle
					const float Angle = (j / (OrbitLineSegments - 1)) * PI * 2;

					//Add Orbit Position
					KeplerianBodiesOrbitPositionss[i].Add(MathGetOrbitPositionInclined(Angle, OrbitSemiMajorLength, OrbitSemiMinorLength, OrbitInclination, OrbitCenterX , OrbitCenterY));
				}

				//Get Orbit Body Positions
				{
					//Get Mean Anomaly
					const double MeanAnomaly = (SimulationTime / KeplerianBodiesOrbitPeriods[i]) * PI * 2;
					
					//Set Position
					KeplerianBodiesPositionss[i] = MathGetOrbitPositionInclined(MeanAnomaly, OrbitSemiMajorLength, OrbitSemiMinorLength, OrbitInclination, OrbitCenterX , OrbitCenterY);
				}

				// Get Keplerian Body Parent Positions
				{
					KeplerianBodiesParentPositionss[i] = FVector::ZeroVector;
					
					for (int j = 0; j < FLUID_ASTRO_KEPLERIAN_BODIES_MAX; j++)
					{
						// Skip If Not Active
						if (!bIsKeplerianBodiesActives[i])
							continue;
						
						if (KeplerianBodiesParentIds[i] == KeplerianBodiesIds[j])
							KeplerianBodiesParentPositionss[i] =  KeplerianBodiesPositionss[j] + KeplerianBodiesParentPositionss[j];
					}
				}
			}

			bIsKeplerianBodiesDirty = false;
		}
	}
}

void UFluidAstroKeplerianBodiesSubsystem::SetSimulateOutputPositionScale(const float Value)
{
	SimulationOutputPositionScale = Value * 0.00000001f;
}

void UFluidAstroKeplerianBodiesSubsystem::GetSimulateOutputPositionScale(float& Value_Out)
{
	Value_Out = SimulationOutputPositionScale;
}

void UFluidAstroKeplerianBodiesSubsystem::SetSimulateOutputRadiusScale(const float Value)
{
	SimulationOutputRadiusScale = Value * 0.00000001f;
}

void UFluidAstroKeplerianBodiesSubsystem::GetSimulateOutputRadiusScale(float& Value_Out)
{
	Value_Out = SimulationOutputRadiusScale;
}

/*=============================================================================
Fluid Astro KeplerianBodies Subsystem : Debug
=============================================================================*/

void UFluidAstroKeplerianBodiesSubsystem::DebugDraw()
{
	//Do Not Debug Draw
	if (CDebugFluidAstroKeplerianBodiesDebugDraw.GetValueOnAnyThread() <= 0)
		return;

	//Get World
	UWorld* World = GetWorld();

	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidAstroKeplerianBodies_DebugDraw);
	{
		for (int i = 0; i < FLUID_ASTRO_KEPLERIAN_BODIES_MAX; i++)
		{
			if (!bIsKeplerianBodiesActives[i])
				continue;

			const FColor Color = KeplerianBodiesColors[i];
			const FVector ParentPosition = KeplerianBodiesParentPositionss[i];

			//Draw Orbit
			for (int j = 1; j < KeplerianBodiesOrbitPositionss[i].Num(); j++)
			{
				FVector A = (KeplerianBodiesOrbitPositionss[i][j] + ParentPosition);
				A.X *= SimulationOutputPositionScale;
				A.Y *= SimulationOutputPositionScale;
				A.Z *= SimulationOutputPositionScale;
				FVector B = (KeplerianBodiesOrbitPositionss[i][j - 1] + ParentPosition);
				B.X *= SimulationOutputPositionScale;
				B.Y *= SimulationOutputPositionScale;
				B.Z *= SimulationOutputPositionScale;
				DrawDebugLine(World,A,B, Color);
			}

			//Draw Body
			FVector A = KeplerianBodiesParentPositionss[i] + KeplerianBodiesPositionss[i];
			A.X *= SimulationOutputPositionScale;
			A.Y *= SimulationOutputPositionScale;
			A.Z *= SimulationOutputPositionScale;
			DrawDebugSphere(World,A, KeplerianBodiesBodyRadiuses[i], 20, Color);
		}
	}
}

/*=============================================================================
Fluid Astro KeplerianBodies Subsystem : Math
=============================================================================*/


FVector UFluidAstroKeplerianBodiesSubsystem::MathGetOrbitPositionInclined(const float Anomaly, const float SemiMajorLength, const float SemiMinorLength, const float Inclination, const float CenterX, float const CenterY)
{
	//Position
	FVector Position = FVector::Zero();
	const double PositionX = Position.X = FMath::Cos(Anomaly) * SemiMajorLength;
	const double PositionY = Position.Y = (FMath::Sin(Anomaly) * SemiMinorLength) + CenterY;

	//Inclination
	Position.Y = FMath::Sin(Inclination) * PositionX;
	Position.X = FMath::Cos(Inclination) * PositionY + CenterX;
	
	//Return Position
	return Position;
}



