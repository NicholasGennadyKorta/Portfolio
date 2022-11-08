// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidEntitiesBenchmarkSubsystem.h"

#include "BehaviorTree/BehaviorTreeTypes.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Subsystem Main

void UFluidEntitiesBenchmarkSubsystem::InitializeEntities()
{
	//Return If Already Initialized | Set Initialized
	if (bInitialized) return;
	bInitialized = true;

	//Get Core Subsystem | Entities Manager
	auto CoreSubsystem = GetGameInstance()->GetSubsystem<UFluidEntitiesCoreSubsystem>();
	EntitiesManager = CoreSubsystem->EntitiesManager;
	
	//Register Entities Components
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FVector,"BenchmarkPositionComponents");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FVector,"BenchmarkDirectionComponents");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FQuat,"BenchmarkRotationsComponents");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FLinearColor,"BenchmarkColorsComponents");
	
	//Get Pointers To Components
	PositionComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FVector,"BenchmarkPositionComponents");
	DirectionComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FVector,"BenchmarkDirectionComponents");
	RotationComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FQuat,"BenchmarkRotationsComponents");
	ColorComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FLinearColor,"BenchmarkColorsComponents");

	//Components Copy TArray
	PositionComponentsCopyTArray.AddDefaulted(FLUID_ENTITIES_CORE_ENTITIES_MAX);
	RotationComponentsCopyTArray.AddDefaulted(FLUID_ENTITIES_CORE_ENTITIES_MAX);
	ColorComponentsCopyTArray.AddDefaulted(FLUID_ENTITIES_CORE_ENTITIES_MAX);

	//Init Entities Character System States
	EntitiesCharacterSystemStates.Init(false,3);
}

void UFluidEntitiesBenchmarkSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initalized
	if (!bInitialized) return;

	//Do Systems
	PositionEntitiesSystem(DeltaTime);
	PositionEntitiesSystemAsync(DeltaTime);
	RotationEntitiesSystem(DeltaTime);
	ColorEntitiesSystem(DeltaTime);
}

bool UFluidEntitiesBenchmarkSubsystem::IsTickable() const
{
	return !IsTemplate() && bInitialized;
}

bool UFluidEntitiesBenchmarkSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidEntitiesBenchmarkSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidEntitiesBenchmarkSubsystem::GetStatId() const
{
	return TStatId();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Benchmark Subsystem Entities Systems

void UFluidEntitiesBenchmarkSubsystem::PositionEntitiesSystem(const float DeltaTime)
{
	//Return If Not Enabled
	if (!EntitiesCharacterSystemStates[0] || bIsAsyncEnabledForSystems) return;
	
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidEntitiesBenchmark_Move);
	{
		//Do Task
		{
			const int Bounds = 6666;
			
			for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
			{
				//Not A Benchmark Entity Type So Return
				if (!BenchamrkEntitiesSignatures[i])
					continue;
			
				//Get Components
				const auto Direction = &DirectionComponents[i];
				const auto Position = &PositionComponents[i];
			
				//Reverse Direction
				if (Position->X < -Bounds || Position->X > Bounds)
					Direction->X *= -1;
				if (Position->Y < -Bounds || Position->Y > Bounds)
					Direction->Y *= -1;
				if (Position->Z < -Bounds || Position->Z > Bounds)
					Direction->Z *= -1;

				//Move
				Position->X += Direction->X * DeltaTime * 800;
				Position->Y += Direction->Y * DeltaTime * 800;
				Position->Z += Direction->Z * DeltaTime * 800;

				//Copy Over Position To Position TArray
				PositionComponentsCopyTArray[i] = *Position;
			}
		}
	}
}

void UFluidEntitiesBenchmarkSubsystem::PositionEntitiesSystemAsync(const float DeltaTime)
{
	//Return If Not Enabled
	if (!EntitiesCharacterSystemStates[0] || !bIsAsyncEnabledForSystems) return;
	
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidEntitiesBenchmark_Move_Async);
	{
		//Task Parameters
		auto SystemIn = this;
		auto BenchamrkEntitiesSignaturesIn = BenchamrkEntitiesSignatures;
		auto DirectionComponentsIn = DirectionComponents;
		auto PositionComponentsIn = PositionComponents;
		
		//Do Task
		AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [SystemIn, BenchamrkEntitiesSignaturesIn,DirectionComponentsIn,PositionComponentsIn, DeltaTime] ()
		{
			const int Bounds = 6666;
			
			for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
			{
				//Not A Benchmark Entity Type So Return
				if (!BenchamrkEntitiesSignaturesIn[i])
					continue;
			
				//Get Components
				const auto Direction = &DirectionComponentsIn[i];
				const auto Position = &PositionComponentsIn[i];
			
				//Reverse Direction
				if (Position->X < -Bounds || Position->X > Bounds)
					Direction->X *= -1;
				if (Position->Y < -Bounds || Position->Y > Bounds)
					Direction->Y *= -1;
				if (Position->Z < -Bounds || Position->Z > Bounds)
					Direction->Z *= -1;

				//Move
				Position->X += Direction->X * DeltaTime * 800;
				Position->Y += Direction->Y * DeltaTime * 800;
				Position->Z += Direction->Z * DeltaTime * 800;

				//Copy Over Position To Position TArray
				SystemIn->PositionComponentsCopyTArray[i] = *Position;
			}
		});
	}
}

void UFluidEntitiesBenchmarkSubsystem::RotationEntitiesSystem(const float DeltaTime)
{
}

void UFluidEntitiesBenchmarkSubsystem::ColorEntitiesSystem(const float DeltaTime)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Benchmark Subsystem Entities Systems States

void UFluidEntitiesBenchmarkSubsystem::SetEntitiesSystemStates(const bool bIsMoveSystemEnabled, const bool bIsRotateSystemEnabled, const bool bIsColorSystemEnabled, const bool bIsAsyncEnabled)
{
	//Set States
	SetEntitiesMoveSystemState(bIsMoveSystemEnabled);
	SetEntitiesRotateSystemState(bIsRotateSystemEnabled);
	SetEntitiesColorSystemState(bIsColorSystemEnabled);
	bIsAsyncEnabledForSystems = bIsAsyncEnabled;
}

void UFluidEntitiesBenchmarkSubsystem::SetEntitiesMoveSystemState(const bool bIsEnabled)
{
	//Initialize Entities If Needed
	InitializeEntities();

	EntitiesCharacterSystemStates[0] = bIsEnabled;
}

void UFluidEntitiesBenchmarkSubsystem::SetEntitiesRotateSystemState(const bool bIsEnabled)
{
	//Initialize Entities If Needed
	InitializeEntities();

	EntitiesCharacterSystemStates[1] = bIsEnabled;
}

void UFluidEntitiesBenchmarkSubsystem::SetEntitiesColorSystemState(const bool bIsEnabled)
{
	//Initialize Entities If Needed
	InitializeEntities();
	
	EntitiesCharacterSystemStates[2] = bIsEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Benchmark Subsystem Entities Signatures

void UFluidEntitiesBenchmarkSubsystem::SetEntityToEntityBenchmarkSignature(int Entity, bool bValue)
{
	BenchamrkEntitiesSignatures[Entity] = bValue;
}

void UFluidEntitiesBenchmarkSubsystem::SetEntitiesToEntityBenchmarkSignature(TArray<int> Entities, bool bValue)
{
	for (int i = 0; i < Entities.Num(); i++)
		if (Entities[i] >= 0 && Entities[i] < FLUID_ENTITIES_CORE_ENTITIES_MAX)
			BenchamrkEntitiesSignatures[Entities[i]] = bValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Benchmark Subsystem Entities Set Components TArray

void UFluidEntitiesBenchmarkSubsystem::SetPositionComponentsByTArray(TArray<FVector> Components)
{
	for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		if (BenchamrkEntitiesSignatures[i])
			PositionComponents[i] = Components[i];
}

void UFluidEntitiesBenchmarkSubsystem::SetDirectionComponentsByTArray(TArray<FVector> Components)
{
	for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		if (BenchamrkEntitiesSignatures[i])
			DirectionComponents[i] = Components[i];
}

void UFluidEntitiesBenchmarkSubsystem::SetRotationComponentsByTArray(TArray<FQuat> Components)
{
	for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		if (BenchamrkEntitiesSignatures[i])
			RotationComponents[i] = Components[i];
}

void UFluidEntitiesBenchmarkSubsystem::SetColorComponentsByTArray(TArray<FColor> Components)
{
	for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		if (BenchamrkEntitiesSignatures[i])
			ColorComponents[i] = Components[i];
}


