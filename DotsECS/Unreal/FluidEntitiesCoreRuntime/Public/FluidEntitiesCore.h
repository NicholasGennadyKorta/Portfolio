#pragma once

#include <iostream>
#include<string>
#include <map>
#include <queue>
#include <vector>

struct IFluidEntitiesCoreComponent;

//Max Entities | Component Count
const int FLUID_ENTITIES_CORE_ENTITIES_MAX = 100000;

//Entities Core
class FluidEntitiesCoreManager
{
//Entities Manager Systems
public:

	void Initialize();

	bool bIsAlreadyInitialized;

//Entities Manager Entities
public:
    
	void InitializeEntities();
	void CreateEntity(int& Entity_Out);
	void CreateEntities(int Count, std::vector<int>& Entities_Out);
	void DestroyEntity(int Entity);
	
	std::queue<int> EntitiesAvailable;
	int EntitiesCount;

//Entities Manager Components
public:

	std::map<std::string,int> EntitiesComponentRegisteredIDsToSignatureIndex;
	std::map<std::string,void*> EntitiesComponentPairs;
	std::vector<std::string> EntitiesComponentRegisteredIDs;
	int EntitiesComponentRegisteredAmount;

	//Entities Component Type DECLARATION
	#define FLUID_ENTITIES_CORE_COMPONENT_TYPE_DECLARATION(T,ID) struct { T *data;  }

	//Entities Component Initialize Macro
	#define FLUID_ENTITIES_CORE_COMPONENT_INITIALIZE(M,T,COMPS,ID) do { \
	(COMPS).data = (T*)malloc((FLUID_ENTITIES_CORE_ENTITIES_MAX) * sizeof(*(COMPS).data)); \
	M->EntitiesComponentPairs.insert(M->EntitiesComponentPairs.begin(), std::pair<std::string,void*> (ID,&(COMPS).data[0])); \
	} while (0)

	//Entities Component Register Macro
	#define FLUID_ENTITIES_CORE_COMPONENT_REGISTER(M,T,ID) do { \
	FLUID_ENTITIES_CORE_COMPONENT_TYPE_DECLARATION(T,ID) COMPS;\
	FLUID_ENTITIES_CORE_COMPONENT_INITIALIZE(M,T,COMPS,ID); \
	M->EntitiesComponentRegisteredIDs.push_back(ID); \
	M->EntitiesComponentRegisteredIDsToSignatureIndex.insert(M->EntitiesComponentRegisteredIDsToSignatureIndex.begin(), std::pair<std::string,int> (ID,M->EntitiesComponentRegisteredAmount)); \
	M->EntitiesComponentRegisteredAmount++; \
	} while(0)

	//Entities Component Get
	#define FLUID_ENTITIES_CORE_COMPONENTS(M,T,ID) (T*)M->EntitiesComponentPairs[ID]
};