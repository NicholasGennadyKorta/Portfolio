// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FluidEntitiesCoreRuntime.h"
#include "FluidEntitiesCoreSubsystem.generated.h"

//Fluid Entities Core Bindings
USTRUCT(BlueprintType)
struct FLUIDENTITIESCORERUNTIME_API FFluidEntitiesCoreBindings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
        TMap<FName,UObject*> ObjectBindings;
    UPROPERTY(BlueprintReadWrite)
        TMap<FName,FName> ObjectBindingTags;
};

//Fluid Entities Core Subsystem
UCLASS()
class FLUIDENTITIESCORERUNTIME_API UFluidEntitiesCoreSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Entities Core Subsystem Main
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

    bool bInitialized;
    FluidEntitiesCoreManager* EntitiesManager;
    
//Fluid Entities Core Subsystem Binding
public:

    UFUNCTION(BlueprintCallable)
        void AddEntityObjectBinding(int Entity, UObject* Object, FName Name, FName Tag);
    UFUNCTION(BlueprintCallable)
        void RemoveEntityObjectBinding(int Entity, FName Name,bool bDestroyObject);
    UFUNCTION(BlueprintCallable)
        void RemoveEntityObjectBindings(int Entity, bool bDestroyObjects);
    UFUNCTION(BlueprintCallable)
        void GetEntityObjectBinding(int Entity, FName Name, UObject*& Object_Out, FName& Tag_Out);
    UFUNCTION(BlueprintCallable)
        void HasEntityObjectBinding(int Entity, FName Name, bool& bHasBinding_Out);

    FFluidEntitiesCoreBindings Bindings[FLUID_ENTITIES_CORE_ENTITIES_MAX];
    
    
//Fluid Entities Core Subsystem Tags
public:
   
    UFUNCTION(BlueprintCallable)
        void AddEntityTag(const int Entity, const FName Tag);
    UFUNCTION(BlueprintCallable)
        void RemoveEntityTag(const int Entity, const FName Tag);
    UFUNCTION(BlueprintCallable)
        void RemoveEntityTags(const int Entity);
    UFUNCTION(BlueprintCallable)
        void GetEntityFirstWithTag(const FName Tag, int& Entity_Out);
    UFUNCTION(BlueprintCallable)
        void GetEntitiesWithTag(const FName Tag, TArray<int>& Entities_Out);
    UFUNCTION(BlueprintCallable)
        void HasEntityTag(const int Entity, const FName Tag, bool& bHasTag_Out);
        
    TMap<FName,FName> Tags[FLUID_ENTITIES_CORE_ENTITIES_MAX];

//Fluid Entities Core Subsystem Entities Create | Destroy
public:
    
    UFUNCTION(BlueprintCallable)
        void CreateEntity(int& Entity_Out);
    UFUNCTION(BlueprintCallable)
        void CreateEntities(int Count, TArray<int>& Entities_Out);
    UFUNCTION(BlueprintCallable)
        void DestroyEntity(int Entity);

//Fluid Entities Core Subsystem Entities Components
public:
    
    UFUNCTION(BlueprintCallable)
        void GetEntityComponentRegisteredAmount(int& Amount_Out);
    UFUNCTION(BlueprintCallable)
        void GetEntityComponentRegisteredIDs(TArray<FString>& IDs_Out);
    
//Fluid Entities Core Subsystem Utility
public:
	
    UFUNCTION(BlueprintCallable)
        void UtilityGetWorld(UWorld*& World_Out);
    UFUNCTION(BlueprintCallable)
        void UtilitySpawnActorFromClass(UClass* Class, FVector Location, FRotator Rotation, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, AActor*& Actor_Out);
    UFUNCTION(BlueprintCallable)
        void UtilityCreateSceneComponentFromClass(UClass* Class,UObject* Outer, USceneComponent*& SceneComponent_Out);
};