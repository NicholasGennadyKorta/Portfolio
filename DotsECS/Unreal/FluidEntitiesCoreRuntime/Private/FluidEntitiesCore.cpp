#include "FluidEntitiesCore.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// Entities Manager Main

void FluidEntitiesCoreManager::Initialize()
{
	//Initialize Once
	if (bIsAlreadyInitialized) return;
	InitializeEntities();
	bIsAlreadyInitialized = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Entities Manager Entities

void FluidEntitiesCoreManager::InitializeEntities()
{
	// Initialize Entity Available Queue With All Possible Entities
	for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; ++i)
		EntitiesAvailable.push(i);
}

void FluidEntitiesCoreManager::CreateEntity(int& Entity_Out)
{
	//Return If We Reached Max Entity Count
	if (EntitiesCount >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Entity_Out = -1;
		return;
	}
    
	// Take A Entity From The Front Of Available Queue
	Entity_Out = EntitiesAvailable.front();
	EntitiesAvailable.pop();
	++EntitiesCount;
}

void FluidEntitiesCoreManager::CreateEntities(int Count, std::vector<int>& Entities_Out)
{
	//Create  Entities
	int Entity = -1;
	for (int i = 0; i < Count; i++)
	{
		CreateEntity(Entity);
		if (Entity != -1) Entities_Out.push_back(Entity);
		else return;
	}
}

void FluidEntitiesCoreManager::DestroyEntity(int Entity)
{
	//Return If Is Not Valid
	if (Entity < 0 ||Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;
	
	// Put Destroyed Entity Back In Available Que (At The Back) | Decrease Entity Count
	EntitiesAvailable.push(Entity);
	--EntitiesCount;
}