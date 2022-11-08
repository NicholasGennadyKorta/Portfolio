// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FluidEntitiesCoreRuntime.h"
#include "FluidEntitiesBenchmarkSubsystem.generated.h"

//Forward Declarations
class FluidEntitiesCoreManager;
class UFluidEntitiesCoreSubsystem;

//Fluid Entities Benchmark Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid Entities Benchmark"), STATGROUP_FluidEntitiesBenchmark, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Benchmark Move"),STAT_FluidEntitiesBenchmark_Move,STATGROUP_FluidEntitiesBenchmark);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Benchmark Move(Async)"),STAT_FluidEntitiesBenchmark_Move_Async,STATGROUP_FluidEntitiesBenchmark);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Benchmark Rotate"),STAT_FluidEntitiesBenchmark_Rotate,STATGROUP_FluidEntitiesBenchmark);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Benchmark Color"),STAT_FluidEntitiesBenchmark_Color,STATGROUP_FluidEntitiesBenchmark);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Benchmark Construct Components To TArray (Copy)"),STAT_FluidEntitiesBenchmark_Construct_Components_To_TArray,STATGROUP_FluidEntitiesBenchmark);

//Fluid Entities Benchmark Subsystem
UCLASS()
class FLUIDENTITIESBENCHMARKRUNTIME_API UFluidEntitiesBenchmarkSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Entities Benchmark Subsystem Main
public:
	
	void InitializeEntities();
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	FluidEntitiesCoreManager* EntitiesManager;
	bool bInitialized;
	
//Fluid Entities Benchmark Subsystem Entities Systems
private:

	void PositionEntitiesSystem(const float DeltaTime);
	void PositionEntitiesSystemAsync(const float DeltaTime);
	void RotationEntitiesSystem(const float DeltaTime);
	void ColorEntitiesSystem(const float DeltaTime);

//Fluid Entities Benchmark Subsystem Entities Systems States
public:
	
	UFUNCTION(BlueprintCallable)
		void SetEntitiesSystemStates(const bool bIsMoveSystemEnabled, const bool bIsRotateSystemEnabled, const bool bIsColorSystemEnabled, const bool bIsAsyncEnabled);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesMoveSystemState(const bool bIsEnabled);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesRotateSystemState(const bool bIsEnabled);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesColorSystemState(const bool bIsEnabled);

	TBitArray<> EntitiesCharacterSystemStates;
	bool bIsAsyncEnabledForSystems;

//Fluid Entities Benchmark Subsystem Entities Signatures
public:

	UFUNCTION(BlueprintCallable)
		void SetEntityToEntityBenchmarkSignature(int Entity, bool bValue);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesToEntityBenchmarkSignature(TArray<int> Entities, bool bValue);

	bool BenchamrkEntitiesSignatures[FLUID_ENTITIES_CORE_ENTITIES_MAX];
	
//Fluid Entities Benchmark Subsystem Entities Set Components TArray
public:

	UFUNCTION(BlueprintCallable)
		void SetPositionComponentsByTArray(TArray<FVector> Components);
	UFUNCTION(BlueprintCallable)
		void SetDirectionComponentsByTArray(TArray<FVector> Components);
	UFUNCTION(BlueprintCallable)
		void SetRotationComponentsByTArray(TArray<FQuat> Components);
	UFUNCTION(BlueprintCallable)
		void SetColorComponentsByTArray(TArray<FColor> Components);
	
	UPROPERTY(BlueprintReadOnly)
		TArray<FVector> PositionComponentsCopyTArray;
	UPROPERTY(BlueprintReadOnly)
		TArray<FQuat> RotationComponentsCopyTArray;
	UPROPERTY(BlueprintReadOnly)
		TArray<FLinearColor> ColorComponentsCopyTArray;
	
	FVector* PositionComponents;
	FVector* DirectionComponents;
	FQuat* RotationComponents;
	FLinearColor* ColorComponents;
};