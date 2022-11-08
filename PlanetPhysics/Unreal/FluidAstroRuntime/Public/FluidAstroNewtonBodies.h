// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>
#include <map>
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/DataTable.h"
#include "FluidAstroNewtonBodies.generated.h"

//Fluid Astro NewtonBodies Physics Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid Astro Newton Bodies"), STATGROUP_FluidAstroNewtonBodies, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid Astro Newton Bodies: Simulate Gravity / Law Of Attraction"), STAT_FluidAstroNewtonBodies_SimulateGravityLawOfAttraction, STATGROUP_FluidAstroNewtonBodies);
DECLARE_CYCLE_STAT(TEXT("Fluid Astro Newton Bodies: Simulate Move"), STAT_FluidAstroNewtonBodies_SimulateMove, STATGROUP_FluidAstroNewtonBodies);
DECLARE_CYCLE_STAT(TEXT("Fluid Astro Newton Bodies: Debug Draw Bodies"), STAT_FluidAstroNewtonBodies_DebugDrawCelestialsBodies, STATGROUP_FluidAstroNewtonBodies);
DECLARE_CYCLE_STAT(TEXT("Fluid Astro Newton Bodies: Debug Draw Orbit Lines"), STAT_FluidAstroNewtonBodies_DebugDrawCelestialsOrbitLines, STATGROUP_FluidAstroNewtonBodies);

//Fluid Astro NewtonBodies Physics Constants
const int FLUID_ASTRO_NEWTON_BODIES_MAX = 100000;
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_MASS_SOLAR = 1.98892e+30;
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_MASS_EARTH = 5.9742e+24;
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_MASS_LUNA =  FLUID_ASTRO_NEWTON_BODIES_UNIT_MASS_EARTH * 0.0123;
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_DISTANCE_AU_CM = 149.6e+6 * 1000; //Earth Distance From Sun
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_DISTANCE_AU_LUNA_CM = FLUID_ASTRO_NEWTON_BODIES_UNIT_DISTANCE_AU_CM * 1.00257; 
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_ORBITAL_VELOCITY_EARTH = 29.783 * 1000;
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_ORBITAL_VELOCITY_LUNA = FLUID_ASTRO_NEWTON_BODIES_UNIT_ORBITAL_VELOCITY_EARTH * 1.0343;
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_RADIUS_SOLAR = 6.378e+8 * 109; //Scaled By Earths Earth's;
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_RADIUS_EARTH = 6.378e+8; 
const double FLUID_ASTRO_NEWTON_BODIES_UNIT_RADIUS_LUNAR = 6.378e+8 * 0.27270; //Scaled By Earth's;
const double FLUID_ASTRO_NEWTON_BODIES_SIMULATION_G = 6.67428e-11; // Gravitational Constant In Issac Newton Law Of Universal Gravitation And Albert Einstein's Theory Of General Relativity

//Fluid Astro NewtonBodies Unit Mass
UENUM(Blueprintable)
enum EFluidAstroNewtonBodiesUnitMass
{
	FluidAstroCelestialsSolMass				UMETA(DisplayName = "Sol"),
	FluidAstroCelestialsEarthMass			UMETA(DisplayName = "Earth"),
	FluidAstroCelestialsLunaMass			UMETA(DisplayName = "Luna")
};

//Fluid Astro NewtonBodies Unit Distance
UENUM(Blueprintable)
enum EFluidAstroNewtonBodiesUnitDistance
{
	FluidAstroCelestialsAU					UMETA(DisplayName = "AU(Sun To Earth)"),
	FluidAstroCelestialsAULuna				UMETA(DisplayName = "Luna(Sun To Earth's Moon)")
};

//Fluid Astro NewtonBodies Unit Orbital Velocity
UENUM(Blueprintable)
enum EFluidAstroNewtonBodiesUnitOrbitalVelocity
{
	FluidAstroCelestialsEarthOrbitalVelocity UMETA(DisplayName = "Earth"),
	FluidAstroCelestialsLunaOrbitalVelocity  UMETA(DisplayName = "Luna")
};

//Fluid Astro NewtonBodies Unit Diameter
UENUM(Blueprintable)
enum EFluidAstroNewtonBodiesUnitRadius
{
	FluidAstroCelestialsSolRadius			UMETA(DisplayName = "Sol"),
	FluidAstroCelestialsEarthRadius			UMETA(DisplayName = "Earth"),
	FluidAstroCelestialsLunaRadius			UMETA(DisplayName = "Luna")
};

//Fluid Astro NewtonBodies DataTable Row
USTRUCT(BlueprintType)
struct FFluidAstroNewtonBodiesTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
		FText Name;
	UPROPERTY(EditDefaultsOnly)
		FColor Color;
	UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EFluidAstroNewtonBodiesUnitDistance> UnitDistance;
	UPROPERTY(EditDefaultsOnly)
		double MeanDistance;
	UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EFluidAstroNewtonBodiesUnitOrbitalVelocity> UnitOrbitalVelocity;
	UPROPERTY(EditDefaultsOnly)
		double OrbitalVelocity;
	UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EFluidAstroNewtonBodiesUnitRadius> UnitRadius;
	UPROPERTY(EditDefaultsOnly)
		double Radius;
	UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EFluidAstroNewtonBodiesUnitMass> UnitMass;
	UPROPERTY(EditDefaultsOnly)
		double Mass;
};

//Fluid Astro NewtonBodies Subsystem
UCLASS(BlueprintType)
class FLUIDASTRORUNTIME_API UFluidAstroNewtonBodiesSubsystem : public UGameInstanceSubsystem,  public FTickableGameObject
{
	GENERATED_BODY()
	
//Fluid Astro NewtonBodies Subsystem : Initialize	| Tick
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	bool bInitialized;

//Fluid Astro NewtonBodies Subsystem : Entity Management
public:

	UFUNCTION(BlueprintInternalUseOnly)
		void InitializeNewtonBodies();
	UFUNCTION(BlueprintCallable)
		void AddNewtonBodyRaw(const FText Name, const FColor Color, const double DistanceMean, const double OrbitalVelocity, const double Radius, const double Mass, int& NBody_Out);
	UFUNCTION(BlueprintCallable)
		void AddNewtonBodiesByDataTable(UDataTable* DataTable, TArray<int>& NewtonBodies_Out);
	UFUNCTION(BlueprintCallable)
		void AddNewtonBodiesByDataTableRow(UDataTable* DataTable, const FName RowName, int& NewtonBody_Out);
	UFUNCTION(BlueprintCallable)
		void AddNewtonBodiesByDataTableRows(UDataTable* DataTable, const TArray<FName> RowNames, TArray<int>& NewtonBodies_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyNewtonBody(const int NewtonBody);
	
	FText NewtonBodiesNames[FLUID_ASTRO_NEWTON_BODIES_MAX];
	FColor NewtonBodiesColors[FLUID_ASTRO_NEWTON_BODIES_MAX];
	TArray<double> NewtonBodiesLocationsPreviousArrayXs[FLUID_ASTRO_NEWTON_BODIES_MAX];
	TArray<double> NewtonBodiesLocationsPreviousArrayYs[FLUID_ASTRO_NEWTON_BODIES_MAX];
	double NewtonBodiesLocationsXs[FLUID_ASTRO_NEWTON_BODIES_MAX];;
	double NewtonBodiesLocationsYs[FLUID_ASTRO_NEWTON_BODIES_MAX];;
	double NewtonBodiesVelocityXs[FLUID_ASTRO_NEWTON_BODIES_MAX];;
	double NewtonBodiesVelocityYs[FLUID_ASTRO_NEWTON_BODIES_MAX];;
	double NewtonBodiesMeanDistances[FLUID_ASTRO_NEWTON_BODIES_MAX];;
	double NewtonBodiesRadiuses[FLUID_ASTRO_NEWTON_BODIES_MAX];;
	double NewtonBodiesMasses[FLUID_ASTRO_NEWTON_BODIES_MAX];;
	bool bIsNewtonBodiesActives[FLUID_ASTRO_NEWTON_BODIES_MAX];;

	TArray<int> NewtonBodiesCreated;
	std::queue<int> NewtonBodiesAvailable;
	int NewtonBodiesCount;

//Fluid Astro NewtonBodies Subsystem : Simulation
public:
	
	UFUNCTION(BlueprintCallable)
		void SimulateInitialize();
	UFUNCTION(BlueprintCallable)
		void SimulateTick(const float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void SetSimulateOutputPositionScale(const float Value);
	UFUNCTION(BlueprintCallable)
		void GetSimulateOutputPositionScale(float& Value_Out);
	UFUNCTION(BlueprintCallable)
		void SetSimulateOutputRadiusScale(const float Value);
	UFUNCTION(BlueprintCallable)
		void GetSimulateOutputRadiusScale(float& Value_Out);
	UFUNCTION(BlueprintCallable)
		void SetSimulateTimeStep(const float Value);
	UFUNCTION(BlueprintCallable)
		void GetSimulateTimeStep(float& Value_Out) const;
	
	double SimulationOutputPositionScale;
	double SimulationOutputRadiusScale;
	double SimulationTimeStep;

//Fluid Astro NewtonBodies Subsystem : Debug
public:
	UFUNCTION(BlueprintCallable)
		void DebugDraw();

	int DebugDrawOrbitLineSteps;
};