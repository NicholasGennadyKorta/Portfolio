// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidEntitiesCharacterHybridRuntime.h"
#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameFramework/Character.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FluidEntitiesCharacterHybridSubsystem.generated.h"

//Forward Declarations
class FluidEntitiesCoreManager;
class UFluidEntitiesCoreSubsystem;
class UFluidEntitiesCharacterHybridSubsystem;

//Fluid Entities Character Hybrid Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid Entities Character Hybrid"), STATGROUP_FluidEntitiesCharacter, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Character Actor Binding"),STAT_FluidEntitiesCharacter_ActorBinding,STATGROUP_FluidEntitiesCharacter);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Character Character Binding"),STAT_FluidEntitiesCharacter_CharacterBinding,STATGROUP_FluidEntitiesCharacter); 
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Character Hybrid Input | Movement"),STAT_FluidEntitiesCharacter_Input,STATGROUP_FluidEntitiesCharacter);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Character Hybrid Distance Matching"),STAT_FluidEntitiesCharacter_DistanceMatching,STATGROUP_FluidEntitiesCharacter);
DECLARE_CYCLE_STAT(TEXT("Fluid Entities Character Hybrid Orientation Warping"),STAT_FluidEntitiesCharacter_OrientationWarping,STATGROUP_FluidEntitiesCharacter);

//Fluid Entities Character Hybrid Character
UCLASS()
class FLUIDENTITIESCHARACTERHYBRIDRUNTIME_API AFluidEntitiesCharacterHybridCharacter : public ACharacter
{
	GENERATED_BODY()

//Fluid Entities Character Hybrid Character Main
public:

	AFluidEntitiesCharacterHybridCharacter();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintReadOnly)
		int FluidEntitiesCharacterHybridEntity;
	UPROPERTY(BlueprintReadOnly)
		UFluidEntitiesCharacterHybridSubsystem* FluidEntitiesCharacterHybridSubsystem;

//Fluid Entities Character Hybrid Character Helper
public:

	UFUNCTION(BlueprintImplementableEvent)
		void HelperEventEntityCreated();
	UFUNCTION(BlueprintImplementableEvent)
		void HelperEventPossessedByPlayerLocal(APlayerController* PlayerController, int LocalPlayerIndex);
	UFUNCTION(BlueprintCallable)
		void HelperMoveInput(const float X, const float Y);
	UFUNCTION(BlueprintCallable)
		void HelperMoveDirection(FVector2D Direction, const float Gait);
	UFUNCTION(BlueprintCallable)
		void HelperMoveToLocation(FVector Location, const float Gait);
	UFUNCTION(BlueprintCallable)
		void HelperToggleEntityMovementComponentStrafe();
	UFUNCTION(BlueprintCallable)
		void HelperSetActorMovementJumpProperties(const float GravityScale, const float BrakingDecelerationFalling, const float AirControl, const float AirControlBoostMultiplier, const float AirControlBoostVelocityThreshold, const float FallingLateralFriction, const float JumpMaxTime, const float JumpZVelocity);
};

//Fluid Entities Character Hybrid Subsystem
UCLASS()
class FLUIDENTITIESCHARACTERHYBRIDRUNTIME_API UFluidEntitiesCharacterHybridSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Entities Character Hybrid Subsystem Main
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	
	FluidEntitiesCoreManager* EntitiesManager;
	bool bInitialized;
	
//Fluid Entities Character Hybrid Entities Systems
public:

	void EntitiesCharacterActorBindingsSystem(const float DeltaTime);
	void EntitiesCharacterCharacterBindingsSystem(const float DeltaTime);
	void EntitiesCharacterInputSystem(const float DeltaTime);
	void EntitiesCharacterDistanceMatchingSystem(const float DeltaTime);
	void EntitiesCharacterOrientationWarpingSystem(const float DeltaTime);

//Fluid Entities Character Hybrid Entities Create Destroy
public:

	UFUNCTION(BlueprintCallable)
		void CreateEntity(AActor* ActorToBindTo, AFluidEntitiesCharacterHybridCharacter* FluidEntitiesCharacterHybridCharacterToBindTo, int& Entity_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyEntity(const int Entity);
	UFUNCTION(BlueprintCallable)
		void GetEntityActorBindingEntity(AActor* Actor, int& Entity_Out);
	UFUNCTION(BlueprintCallable)
		void GetEntityCharacterBindingEntity(AFluidEntitiesCharacterHybridCharacter* FluidEntitiesCharacterHybridCharacter, int& Entity_Out);

	TMap<AActor*,int> EntityActorBindingToEntitys;
	TMap<AFluidEntitiesCharacterHybridCharacter*,int> EntityCharacterBindingToEntitys;
	AActor* EntityActorBindings[FLUID_ENTITIES_CORE_ENTITIES_MAX];
	AFluidEntitiesCharacterHybridCharacter* EntityCharacterBindings[FLUID_ENTITIES_CORE_ENTITIES_MAX];
	
//Fluid Entities Character Hybrid Entities Systems States
public:
	
	UFUNCTION(BlueprintCallable)
		void SetEntitiesSystemStates(bool bIsInputSystemEnabled, bool bIsDistanceMatchingSystemEnabled,bool bIsOrientationWarpingSystemEnabled, bool bIsInteractionSystemEnabled);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesInputSystemState(bool bIsEnabled);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesDistanceMatchingSystemState(bool bIsEnabled);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesOrientationWarpingSystemStates(bool bIsEnabled);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesInteractionSystemState(bool bIsEnabled);

	TBitArray<> EntitiesCharacterSystemStates;

//Fluid Entities Character Hybrid Entities Signatures
public:

	UFUNCTION(BlueprintCallable)
		void SetEntityToEntityCharacterHybridSignature(int Entity, bool bValue);
	UFUNCTION(BlueprintCallable)
		void SetEntitiesToEntityCharacterHybridSignature(TArray<int> Entities, bool bValue);

	bool CharacterHybridEntitiesSignatures[FLUID_ENTITIES_CORE_ENTITIES_MAX];
	
//Fluid Entities Character Hybrid Entities Set Components
public:
	
	UFUNCTION(BlueprintCallable)
		void SetEntityTransformComponent(const int Entity, const FFluidEntitiesCharacterHybridTransformComponent Component);
	UFUNCTION(BlueprintCallable)
		void SetEntityInputComponentInput(const int Entity, const float X, const float Y);
	UFUNCTION(BlueprintCallable)
		void SetEntityInputStateComponentLocalPlayerIndex(const int Entity, const int LocalPlayerIndex);
	UFUNCTION(BlueprintCallable)
		void SetEntityInputStateComponentIsDisabled(const int Entity, const bool bIsDisabled);
	UFUNCTION(BlueprintCallable)
		void SetEntityDistanceMatchingComponentInput(const int Entity,const FVector Location, const FVector Velocity, const float BrakingDeceleration, const int Type);
	UFUNCTION(BlueprintCallable)
		void SetEntityOrientationWarpingComponentInput(const int Entity,const float InterpSpeed, const float AngleMaxStepDelta, const int Type);
	UFUNCTION(BlueprintCallable)
		void SetEntityMovementComponent(const int Entity, const FFluidEntitiesCharacterHybridMovementComponent Component);

//Fluid Entities Character Hybrid Entities Set Components Unsafe
public:
	
	UFUNCTION(BlueprintCallable)
		void SetEntityTransformComponentUnsafe(const int Entity, const FFluidEntitiesCharacterHybridTransformComponent Component);
	UFUNCTION(BlueprintCallable)
		void SetEntityInputComponentInputUnsafe(const int Entity, const float X, const float Y);
	UFUNCTION(BlueprintCallable)
		void SetEntityInputStateComponentLocalPlayerIndexUnsafe(const int Entity, const int LocalPlayerIndex);
	UFUNCTION(BlueprintCallable)
		void SetEntityInputStateComponentIsDisabledUnsafe(const int Entity, const bool bIsDisabled);
	UFUNCTION(BlueprintCallable)
		void SetEntityDistanceMatchingComponentInputUnsafe(const int Entity,const FVector Location, const FVector Velocity, const float BrakingDeceleration, const int Type);
	UFUNCTION(BlueprintCallable)
		void SetEntityOrientationWarpingComponentInputUnsafe(const int Entity,const float InterpSpeed, const float AngleMaxStepDelta, const int Type);
	UFUNCTION(BlueprintCallable)
		void SetEntityMovementComponentUnsafe(const int Entity,const FFluidEntitiesCharacterHybridMovementComponent Component);

//Fluid Entities Character Hybrid Entities Get Components
public:

	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityTransformComponent(const int Entity, FFluidEntitiesCharacterHybridTransformComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityInputXYComponent(const int Entity, FFluidEntitiesCharacterHybridInputXYComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityInputYawComponent(const int Entity, FFluidEntitiesCharacterHybridInputYawComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityInputStateComponent(const int Entity, FFluidEntitiesCharacterHybridInputStateComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityDistanceMatchingComponent(const int Entity, FFluidEntitiesCharacterHybridDistanceMatchingComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityOrientationWarpingComponent(const int Entity, FFluidEntitiesCharacterHybridOrientationWarpingComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityMovementComponent(const int Entity,FFluidEntitiesCharacterHybridMovementComponent& Component_Out);


//Fluid Entities Character Hybrid Entities Get Components Unsafe
public:

	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityTransformComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridTransformComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityInputXYComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridInputXYComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityInputYawComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridInputYawComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityInputStateComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridInputStateComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityDistanceMatchingComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridDistanceMatchingComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityOrientationWarpingComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridOrientationWarpingComponent& Component_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetEntityMovementComponentUnsafe(const int Entity,FFluidEntitiesCharacterHybridMovementComponent& Component_Out);
	
//Fluid Entities Character Hybrid Subsystem Components
public:
	
	FFluidEntitiesCharacterHybridTransformComponent* TransformComponents;
	FFluidEntitiesCharacterHybridInputXYComponent* InputXYComponents;
	FFluidEntitiesCharacterHybridInputYawComponent* InputYawComponents;
	FFluidEntitiesCharacterHybridInputStateComponent* InputStateComponents;
	FFluidEntitiesCharacterHybridDistanceMatchingComponent* DistanceMatchingComponents;
	FFluidEntitiesCharacterHybridOrientationWarpingComponent* OrientationWarpingComponents;
	FFluidEntitiesCharacterHybridMovementComponent* MovementComponents;
};