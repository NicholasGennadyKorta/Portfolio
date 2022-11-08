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
#include "FluidAstroKeplerianBodies.generated.h"

//Fluid Astro KeplerianBodies Physics Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid Astro Keplerian Bodies"), STATGROUP_FluidAstroKeplerianBodies, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid Astro Keplerian Bodies: Update Orbit Inputs"), STAT_FluidAstroKeplerianBodies_UpdateOrbitInputs, STATGROUP_FluidAstroKeplerianBodies);
DECLARE_CYCLE_STAT(TEXT("Fluid Astro Keplerian Bodies: Simulate"), STAT_FluidAstroKeplerianBodies_Simulate, STATGROUP_FluidAstroKeplerianBodies);
DECLARE_CYCLE_STAT(TEXT("Fluid Astro Keplerian Bodies: Debug Draw"), STAT_FluidAstroKeplerianBodies_DebugDraw, STATGROUP_FluidAstroKeplerianBodies);

//Fluid Astro KeplerianBodies Physics Constants
const int FLUID_ASTRO_KEPLERIAN_BODIES_MAX = 100000;
const double FLUID_ASTRO_KEPLERIAN_BODIES_UNIT_DISTANCE_AU_CM = 149.6e+6 * 1000; //Earth Distance From Sun
const double FLUID_ASTRO_KEPLERIAN_BODIES_UNIT_RADIUS_SOLAR = 6.378e+8 * 109; //Scaled By Earths Earth's
const double FLUID_ASTRO_KEPLERIAN_BODIES_UNIT_RADIUS_EARTH = 6.378e+8; 
const double FLUID_ASTRO_KEPLERIAN_BODIES_UNIT_RADIUS_LUNAR = 6.378e+8 * 0.27270;

//Fluid Astro KeplerianBodies DataTable Row
USTRUCT(BlueprintType)
struct FFluidAstroKeplerianBodiesTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
		FText Name;
	UPROPERTY(EditDefaultsOnly)
		FName ParentID;
	UPROPERTY(EditDefaultsOnly)
		FColor Color;
	UPROPERTY(EditDefaultsOnly)
		double OrbitCenterX;
	UPROPERTY(EditDefaultsOnly)
		double OrbitCenterY;
	UPROPERTY(EditDefaultsOnly)
		double OrbitAphelion;
	UPROPERTY(EditDefaultsOnly)
		double OrbitPerihelion;
	UPROPERTY(EditDefaultsOnly)
		double OrbitInclination;
	UPROPERTY(EditDefaultsOnly)
		double OrbitPeriod;
	UPROPERTY(EditDefaultsOnly)
		double BodyRadius;
};

//Fluid Astro Keplerian Oribital Subsystem
UCLASS(BlueprintType)
class FLUIDASTRORUNTIME_API UFluidAstroKeplerianBodiesSubsystem : public UGameInstanceSubsystem,  public FTickableGameObject
{
	GENERATED_BODY()
	
//Fluid Astro Keplerian Bodies Subsystem : Initialize	| Tick
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	bool bInitialized;

//Fluid Astro Keplerian Bodies Subsystem : Entity Management
public:

	UFUNCTION(BlueprintInternalUseOnly)
		void InitializeKeplerianBodies();
	UFUNCTION(BlueprintCallable)
		void AddKeplerianBodyRaw(const FName Name, const FName Id, const FName ParentId, const FColor Color, const double OrbitAphelion, const double OrbitPerihelion, const double OrbitInclination, const double OrbitPeriod, const double OrbitCenterX, const double OrbitCenterY, const double BodyRadius, int& KeplerianBody_Out);
	UFUNCTION(BlueprintCallable)
		void AddKeplerianBodiesByDataTable(UDataTable* DataTable, TArray<int>& KeplerianBodies_Out);
	UFUNCTION(BlueprintCallable)
		void AddKeplerianBodiesByDataTableRow(UDataTable* DataTable, const FName RowName, int& KeplerianBodies_Out);
	UFUNCTION(BlueprintCallable)
		void AddKeplerianBodiesByDataTableRows(UDataTable* DataTable, const TArray<FName> RowNames, TArray<int>& KeplerianBodies_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyKeplerianBody(const int KeplerianBody);
	
	FName KeplerianBodiesNames[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	FName KeplerianBodiesIds[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	FName KeplerianBodiesParentIds[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	FColor KeplerianBodiesColors[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	FVector KeplerianBodiesPositionss[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	FVector KeplerianBodiesParentPositionss[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	TArray<FVector> KeplerianBodiesOrbitPositionss[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitOrbitAphelions[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitOrbitPerihelions [FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitSemiMajorLengths[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitSemiMinorLengths[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitInclinations[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitPeriods[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitCenterXs[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesOrbitCenterYs[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	double KeplerianBodiesBodyRadiuses[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];
	bool bIsKeplerianBodiesActives[FLUID_ASTRO_KEPLERIAN_BODIES_MAX];

	std::queue<int> KeplerianBodiesAvailable;
	int KeplerianBodiesCount;
	bool bIsKeplerianBodiesDirty;

//Fluid Astro Keplerian Bodies Subsystem : Simulation
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
	
	double SimulationOutputPositionScale;
	double SimulationOutputRadiusScale;
	double SimulationTime;
	bool bSimulateIsDirty;

//Fluid Astro Keplerian Bodies Subsystem : Debug
public:
	UFUNCTION(BlueprintCallable)
		void DebugDraw();

//Fluid Astro Keplerian Bodies Subsystem : Debug
public:
	
	UFUNCTION(BlueprintCallable)
		FVector MathGetOrbitPositionInclined(const float Anomaly, const float SemiMajorLength, const float SemiMinorLength, const float Inclination, const float CenterX, float const CenterY);
};