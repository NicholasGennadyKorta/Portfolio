// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAstroNewtonBodies.h"

/*=============================================================================
Fluid Astro NewtonBodies Subsystem : Debug
=============================================================================*/

static TAutoConsoleVariable<int32> CDebugFluidAstroNewtonBodiesDebugDraw(
	TEXT("Fluid.Astro.Newton.Bodies.Debug.Draw"),
	0,
	TEXT("Fluid.Astro.Newton.Bodies.Debug.Draw\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);

/*=============================================================================
Fluid Astro NewtonBodies Subsystem : Initialize | Tick
=============================================================================*/

void UFluidAstroNewtonBodiesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//Initialize Once
	if (bInitialized) return;
	bInitialized = true;

	//Initialize
	InitializeNewtonBodies();
	SimulateInitialize();                      
}

void UFluidAstroNewtonBodiesSubsystem::Deinitialize()
{
	bInitialized = false;

	//for (int i = 0; i < FLUID_ASTRO_CELESTIALS_ENTITIES_MAX; i++)
	//	DestroyEntity(i);
}

void UFluidAstroNewtonBodiesSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;

	//Simulate
	SimulateTick(DeltaTime);

	//Debug Draw
	DebugDraw();
}

bool UFluidAstroNewtonBodiesSubsystem::IsTickable() const
{
	return !IsTemplate() && bInitialized;
}

bool UFluidAstroNewtonBodiesSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidAstroNewtonBodiesSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidAstroNewtonBodiesSubsystem::GetStatId() const
{
	return TStatId();
}

/*=============================================================================
Fluid Astro NewtonBodies Subsystem : Entity Management
=============================================================================*/

void UFluidAstroNewtonBodiesSubsystem::InitializeNewtonBodies()
{
	// Initialize Entity Available Queue With All Possible Entities
	for (int i = 0; i < FLUID_ASTRO_NEWTON_BODIES_MAX; ++i)
		NewtonBodiesAvailable.push(i);
}

void UFluidAstroNewtonBodiesSubsystem::AddNewtonBodyRaw(const FText Name, const FColor Color, const double DistanceMean, const double OrbitalVelocity, const double Radius, const double Mass, int& NBody_Out)
{
	//Return If We Reached Max Entity Count
	if (NewtonBodiesCount >= FLUID_ASTRO_NEWTON_BODIES_MAX)
	{
		NBody_Out = -1;
		return;
	}
    
	// Take A Entity From The Front Of Available Queue
	NBody_Out = NewtonBodiesAvailable.front();
	NewtonBodiesAvailable.pop();
	++NewtonBodiesCount;
	NewtonBodiesCreated.Add(NBody_Out);
	
	//Add Entity Data
	NewtonBodiesNames[NBody_Out] = Name;
	NewtonBodiesColors[NBody_Out] = Color;
	NewtonBodiesLocationsXs[NBody_Out] = DistanceMean;
	NewtonBodiesLocationsYs[NBody_Out] = 0;
	NewtonBodiesVelocityXs[NBody_Out] = 0;
	NewtonBodiesVelocityYs[NBody_Out] = OrbitalVelocity;
	NewtonBodiesMeanDistances[NBody_Out] = DistanceMean;
	NewtonBodiesRadiuses[NBody_Out] = Radius;
	NewtonBodiesMasses[NBody_Out] = Mass;
	bIsNewtonBodiesActives[NBody_Out] = true;
}

void UFluidAstroNewtonBodiesSubsystem::AddNewtonBodiesByDataTable(UDataTable* DataTable, TArray<int>& NewtonBodies_Out)
{
	//Return If Not Valid
	if ( DataTable == nullptr)
		return;

	//Add Entities
	AddNewtonBodiesByDataTableRows(DataTable,DataTable->GetRowNames(),NewtonBodies_Out);
}

void UFluidAstroNewtonBodiesSubsystem::AddNewtonBodiesByDataTableRow(UDataTable* DataTable, const FName RowName, int& NewtonBody_Out)
{
	//Get Row
	const FFluidAstroNewtonBodiesTableRow* Row = DataTable->FindRow<FFluidAstroNewtonBodiesTableRow>(RowName,TEXT(""));
	if (!Row)
	{
		NewtonBody_Out = -1;
		return;
	}

	//Calculate Distance
	double Distance = Row->MeanDistance;
	if (Row->UnitDistance == FluidAstroCelestialsAU) Distance *= FLUID_ASTRO_NEWTON_BODIES_UNIT_DISTANCE_AU_CM;
	if (Row->UnitDistance == FluidAstroCelestialsAULuna) Distance *= FLUID_ASTRO_NEWTON_BODIES_UNIT_DISTANCE_AU_LUNA_CM;

	//Calculate Orbital Velocity
	double OrbitalVelocity = Row->OrbitalVelocity;
	if (Row->UnitOrbitalVelocity == FluidAstroCelestialsEarthOrbitalVelocity) OrbitalVelocity *= FLUID_ASTRO_NEWTON_BODIES_UNIT_ORBITAL_VELOCITY_EARTH * -1;
	if (Row->UnitOrbitalVelocity == FluidAstroCelestialsLunaOrbitalVelocity) OrbitalVelocity *= FLUID_ASTRO_NEWTON_BODIES_UNIT_ORBITAL_VELOCITY_LUNA * -1;
	
	//Calculate Mass
	double Mass = Row->Mass;
	if (Row->UnitMass == FluidAstroCelestialsSolMass) Mass *= FLUID_ASTRO_NEWTON_BODIES_UNIT_MASS_SOLAR;
	else if (Row->UnitMass == FluidAstroCelestialsEarthMass) Mass *= FLUID_ASTRO_NEWTON_BODIES_UNIT_MASS_EARTH;
	else if (Row->UnitMass == FluidAstroCelestialsLunaMass) Mass *= FLUID_ASTRO_NEWTON_BODIES_UNIT_MASS_LUNA;

	//Calculate Radius
	double Radius = Row->Radius * 0.5;
	if (Row->UnitRadius == FluidAstroCelestialsSolRadius) Radius *= FLUID_ASTRO_NEWTON_BODIES_UNIT_RADIUS_SOLAR;
	else if (Row->UnitRadius == FluidAstroCelestialsEarthRadius) Radius *= FLUID_ASTRO_NEWTON_BODIES_UNIT_RADIUS_EARTH;
	else if (Row->UnitRadius  == FluidAstroCelestialsLunaRadius) Radius *= FLUID_ASTRO_NEWTON_BODIES_UNIT_RADIUS_LUNAR;

	//Add Entity
	AddNewtonBodyRaw(Row->Name,Row->Color,Distance,OrbitalVelocity,Radius,Mass,NewtonBody_Out);
}

void UFluidAstroNewtonBodiesSubsystem::AddNewtonBodiesByDataTableRows(UDataTable* DataTable, const TArray<FName> RowNames, TArray<int>& NewtonBodies_Out)
{
	//Add Entities
	for (int i = 0; i < RowNames.Num(); i++)
	{
		int Entity = -1;
		AddNewtonBodiesByDataTableRow(DataTable,RowNames[i],Entity);
		if (Entity != -1) NewtonBodies_Out.Add(Entity);
	}
}

void UFluidAstroNewtonBodiesSubsystem::DestroyNewtonBody(const int NewtonBody)
{
	//Return If Entity Not Valid
	if (NewtonBodiesCount >= FLUID_ASTRO_NEWTON_BODIES_MAX)
		return;
	
	//Destroy Entity
	NewtonBodiesAvailable.push(NewtonBody);
	--NewtonBodiesCount;
	NewtonBodiesCreated.Remove(NewtonBody);
	
	//Set Entity Not Active
	bIsNewtonBodiesActives[NewtonBody] = false;
}

/*=============================================================================
Fluid Astro NewtonBodies Subsystem : Simulations
=============================================================================*/

void UFluidAstroNewtonBodiesSubsystem::SimulateInitialize()
{
	//Simulation
	SetSimulateOutputPositionScale(1);
	SetSimulateOutputRadiusScale(1);
	SimulationTimeStep = 3600 * 24; //1 Day
}

void UFluidAstroNewtonBodiesSubsystem::SimulateTick(const float DeltaTime)
{
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidAstroNewtonBodies_SimulateGravityLawOfAttraction);
	{
		//Velocity Of Entity Based On Other Celestials
		{
			//Entity For Forces To Act On
			for (int i = 0; i < NewtonBodiesCount; i++)
			{
				//Entity 1
				const int Entity1 = NewtonBodiesCreated[i];
					
				//Store Our Total Forces Here For Entity 
				double FxTotal = 0;
				double FyTotal = 0;

				//Get Location And Mass Of Entity 1
				const auto L1X = NewtonBodiesLocationsXs[Entity1];
				const auto L1Y = NewtonBodiesLocationsYs[Entity1];
				const auto M1 = NewtonBodiesMasses[Entity1];
				
				//Entities For Forces
				for (int j = 0; j < NewtonBodiesCount; j++)
				{
					//Get Entity 2
					const int Entity2 = NewtonBodiesCreated[j];
						
					//Do Not Act On Itself | Only If Active
					if (Entity1 == Entity2)
						continue;
							
					//Get Location And Mass Of Entity 2
					const auto L2X = NewtonBodiesLocationsXs[Entity2];
					const auto L2Y = NewtonBodiesLocationsYs[Entity2];
					const auto M2 = NewtonBodiesMasses[Entity2];

					//Get Range (Distance) Between Entities
					const double Rx = L2X - L1X;
					const double Ry = L2Y - L1Y;
					const double R = FMath::Sqrt(FMath::Pow(Rx,2) + FMath::Pow(Ry,2));
						
					//Calculate Force (Equation For Universal Gravitation)
					//Equation
					//F=G(m_1m_2) / (r^2)
					//https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation
					const double F = FLUID_ASTRO_NEWTON_BODIES_SIMULATION_G * M1 * M2 / FMath::Pow(R,2);

					//Calculate Force Based On Range (Distance)
					const double Theta = FMath::Atan2(Ry,Rx);
					const double Fx = FMath::Cos(Theta) * F;
					const double Fy = FMath::Sin(Theta) * F;

					//Add To Force Total
					FxTotal += Fx;
					FyTotal += Fy;
				}

				//Calculate Entity 1 Final Velocity From Final Force (As Acceleration) (We Do Not Want To Move Yet As We Must All Update Forces For All Entities At Once)
				//Equation
				//F=MA -> A=F/A
				NewtonBodiesVelocityXs[Entity1] += FxTotal / M1 * SimulationTimeStep * DeltaTime;
				NewtonBodiesVelocityYs[Entity1] += FyTotal / M1 * SimulationTimeStep * DeltaTime;
			}

			//GEngine->AddOnScreenDebugMessage(-1,1,FColor::Red,"FDSFS");
		}
	}

	//Move Our Entities
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidAstroNewtonBodies_SimulateMove);
	{
		for (int i = 0; i < NewtonBodiesCount; i++)
		{
			//Entity
			const int Entity = NewtonBodiesCreated[i];
		
			NewtonBodiesLocationsXs[Entity] += NewtonBodiesVelocityXs[i] * SimulationTimeStep * DeltaTime;
			NewtonBodiesLocationsYs[Entity] += NewtonBodiesVelocityYs[i] * SimulationTimeStep * DeltaTime;
		}
	}
}

void UFluidAstroNewtonBodiesSubsystem::SetSimulateOutputPositionScale(const float Value)
{
	SimulationOutputPositionScale = Value * 0.00000001f;
}

void UFluidAstroNewtonBodiesSubsystem::GetSimulateOutputPositionScale(float& Value_Out)
{
	Value_Out = SimulationOutputPositionScale;
}

void UFluidAstroNewtonBodiesSubsystem::SetSimulateOutputRadiusScale(const float Value)
{
	SimulationOutputRadiusScale = Value * 0.00000001f;
}

void UFluidAstroNewtonBodiesSubsystem::GetSimulateOutputRadiusScale(float& Value_Out)
{
	Value_Out = SimulationOutputRadiusScale;
}

void UFluidAstroNewtonBodiesSubsystem::SetSimulateTimeStep(const float Value)
{
	SimulationTimeStep = Value;
}

void UFluidAstroNewtonBodiesSubsystem::GetSimulateTimeStep(float& Value_Out) const
{
	Value_Out = SimulationTimeStep;
}

/*=============================================================================
Fluid Astro NewtonBodies Subsystem : Debug
=============================================================================*/

void UFluidAstroNewtonBodiesSubsystem::DebugDraw()
{
	//Do Not Debug Draw
	if (CDebugFluidAstroNewtonBodiesDebugDraw.GetValueOnAnyThread() <= 0)
		return;

	//Get World
	UWorld* World = GetWorld();

	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidAstroNewtonBodies_DebugDrawCelestialsBodies);
	{
		//Debug Draw All Celestial Entities Bodies
		for (int i = 0; i < FLUID_ASTRO_NEWTON_BODIES_MAX; i++)
		{
			//Ignore If Entity Is Not Active
			if (!bIsNewtonBodiesActives[i])
				continue;

			//Calculate Debug Draw Parameters
			const double Lx = NewtonBodiesLocationsXs[i] * SimulationOutputPositionScale;
			const double Ly = NewtonBodiesLocationsYs[i] * SimulationOutputPositionScale;
			const float Radius = NewtonBodiesRadiuses[i] * SimulationOutputRadiusScale;

			//Debug Draw
			DrawDebugSphere(World,FVector(Lx,Ly,0),Radius,20,NewtonBodiesColors[i]);
		}
	}
}



