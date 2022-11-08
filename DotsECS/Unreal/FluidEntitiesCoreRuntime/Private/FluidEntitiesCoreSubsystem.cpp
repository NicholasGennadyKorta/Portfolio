// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidEntitiesCoreSubsystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Core Subsystem Main

void UFluidEntitiesCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    if (bInitialized) return;

	EntitiesManager = new FluidEntitiesCoreManager();
	EntitiesManager->Initialize();
	bInitialized = true;
}

void UFluidEntitiesCoreSubsystem::Deinitialize()
{
    bInitialized = false;
}

void UFluidEntitiesCoreSubsystem::Tick(float DeltaTime)
{
    //Return If Not Initalized
    if (!bInitialized) return;
}

bool UFluidEntitiesCoreSubsystem::IsTickable() const
{
	return !IsTemplate() && bInitialized;
}

bool UFluidEntitiesCoreSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidEntitiesCoreSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidEntitiesCoreSubsystem::GetStatId() const
{
	return TStatId();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Core Subsystem Bindings

void UFluidEntitiesCoreSubsystem::AddEntityObjectBinding(int Entity, UObject* Object, FName Name, FName Tag)
{
	//Return If Out Of Bounds
	if (Entity == -1 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Add Object Binding With Name If Not Already Binded
	if (!Bindings[Entity].ObjectBindings.Contains(Name))
	{
		Bindings[Entity].ObjectBindings.Add(Name,Object);
		Bindings[Entity].ObjectBindingTags.Add(Name,Tag);
	}
}

void UFluidEntitiesCoreSubsystem::RemoveEntityObjectBinding(int Entity, FName Name,bool bDestroyObject)
{
	//Return If Out Of Bounds
	if (Entity == -1 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Remove Object Binding With Name
	if (Bindings[Entity].ObjectBindings.Contains(Name))
	{
		auto Object = Bindings[Entity].ObjectBindings[Name];
		Bindings[Entity].ObjectBindings.Remove(Name);
		if (bDestroyObject)
		    Object->ConditionalBeginDestroy();
		Bindings[Entity].ObjectBindingTags.Remove(Name);
	}
}

void UFluidEntitiesCoreSubsystem::RemoveEntityObjectBindings(int Entity, bool bDestroyObjects)
{
	//Return If Out Of Bounds
	if (Entity == -1 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Get Key Array
	TArray<FName> Keys;
	for (const TPair<FName, UObject*>& Pair : Bindings[Entity].ObjectBindings)
		Keys.Add(Pair.Key);

	//Remove Bindings
	while(Keys.Num() > 0)
	{
		RemoveEntityObjectBinding(Entity,Keys[0],bDestroyObjects);
		Keys.RemoveAt(0);
	}
}

void UFluidEntitiesCoreSubsystem::GetEntityObjectBinding(int Entity, FName Name, UObject*& Object_Out, FName& Tag_Out)
{
	//Forece Nullptr By Default
	Object_Out = nullptr;
	
	//Return If Out Of Bounds
	if (Entity == -1 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Return Object Binding With Name
	if (Bindings[Entity].ObjectBindings.Contains(Name))
	{
		Object_Out = Bindings[Entity].ObjectBindings[Name];
		Tag_Out = Bindings[Entity].ObjectBindingTags[Name];
	}
}

void UFluidEntitiesCoreSubsystem::HasEntityObjectBinding(int Entity, FName Name, bool& bHasBinding_Out)
{
	// Force False By Default
	bHasBinding_Out = false;

	//Return If Out Of Bounds
	if (Entity == -1 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Return If Has Binding With Name
	bHasBinding_Out = Bindings[Entity].ObjectBindings.Contains(Name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Core Subsystem Tags

void UFluidEntitiesCoreSubsystem::AddEntityTag(const int Entity, const FName Tag)
{
	if(!Tags[Entity].Contains(Tag))
		Tags[Entity].Add(Tag);
}

void UFluidEntitiesCoreSubsystem::RemoveEntityTag(const int Entity, const FName Tag)
{
	if(Tags[Entity].Contains(Tag))
		Tags[Entity].Remove(Tag);
}

void UFluidEntitiesCoreSubsystem::RemoveEntityTags(const int Entity)
{
	Tags[Entity].Empty();
}

void UFluidEntitiesCoreSubsystem::HasEntityTag(const int Entity, const FName Tag, bool& bHasTag_Out)
{
	bHasTag_Out = false;
	if(Tags[Entity].Contains(Tag))
		bHasTag_Out = true;
}

void UFluidEntitiesCoreSubsystem::GetEntityFirstWithTag(const FName Tag, int& Entity_Out)
{
	Entity_Out = -1;
	for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		if (Tags[i].Contains(Tag))
		{
			Entity_Out = i;
			break;
		}
}

void UFluidEntitiesCoreSubsystem::GetEntitiesWithTag(const FName Tag, TArray<int>& Entities_Out)
{
	Entities_Out.Empty();
	for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		if (Tags[i].Contains(Tag))
			Entities_Out.Add(i);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Core Subsystem Entities Create | Destroy

void UFluidEntitiesCoreSubsystem::CreateEntity(int& Entity_Out)
{
	EntitiesManager->CreateEntity(Entity_Out);	
}

void UFluidEntitiesCoreSubsystem::CreateEntities(int Count, TArray<int>& Entities_Out)
{
	Entities_Out.Empty();
	std::vector<int> Entities;
	EntitiesManager->CreateEntities(Count,Entities);
	for (int i = 0; i < Entities.size(); i++)
		Entities_Out.Add(Entities[i]);
}

void UFluidEntitiesCoreSubsystem::DestroyEntity(int Entity)
{
	EntitiesManager->DestroyEntity(Entity);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Core Subsystem Entities Components

void UFluidEntitiesCoreSubsystem::GetEntityComponentRegisteredAmount(int& Amount_Out)
{
	Amount_Out = EntitiesManager->EntitiesComponentRegisteredAmount;
}

void UFluidEntitiesCoreSubsystem::GetEntityComponentRegisteredIDs(TArray<FString>& IDs_Out)
{
	IDs_Out.Empty();
	for (int i = 0; i < EntitiesManager->EntitiesComponentRegisteredIDs.size(); i++)
		IDs_Out.Add(EntitiesManager->EntitiesComponentRegisteredIDs[i].c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Core Subsystem Utility

void UFluidEntitiesCoreSubsystem::UtilityGetWorld(UWorld*& World_Out)
{
    World_Out = GetWorld();
}

void UFluidEntitiesCoreSubsystem::UtilitySpawnActorFromClass(UClass* Class, FVector Location, FRotator Rotation, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, AActor*& Actor_Out)
{
    Actor_Out = nullptr;
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = SpawnActorCollisionHandlingMethod;
    Actor_Out = GetWorld()->SpawnActor<AActor>(Class, Location, Rotation, SpawnInfo);
}

void UFluidEntitiesCoreSubsystem::UtilityCreateSceneComponentFromClass(UClass* Class,UObject* Outer, USceneComponent*& SceneComponent_Out)
{
	SceneComponent_Out = nullptr;
	SceneComponent_Out = NewObject<USceneComponent>(Outer,Class);
	SceneComponent_Out->RegisterComponent();
}