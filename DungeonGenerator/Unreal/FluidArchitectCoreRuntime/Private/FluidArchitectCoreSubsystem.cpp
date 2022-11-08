// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidArchitectCoreSubsystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Architect Core Profile

void UFluidArchitectCoreProfile::CreateModules(TArray<FName> UIDs, TArray<FName> ViewportNames, TArray<FName> ViewportTypes)
{
	//Modules Data Not Same Length :(
	if (UIDs.Num() != ViewportNames.Num() || UIDs.Num() != ViewportTypes.Num() || ViewportNames.Num() != ViewportTypes.Num())
		return;

	//Variable To Hold If Modules Are Out Of Date
	bool bModulesOutOfDate = false;

	//Modules Not Same Length So There Out Of Date
	if (Modules.Num() != UIDs.Num())
		bModulesOutOfDate = true;

	// Update Modules Viewport Name (If User Changed Hard Coded Name)
	if (!bModulesOutOfDate)
		for (int i = 0; i < UIDs.Num(); i++)
			Modules[i].ViewportName = ViewportNames[i];
	
	//Check If Modules Data Are Same (ID And Type)
	if (!bModulesOutOfDate)
		for (int i = 0; i < Modules.Num(); i++)
			if (Modules[i].UID != UIDs[i] || Modules[i].ViewportType != ViewportTypes[i])
				bModulesOutOfDate = true;

	//Modules Are Out Of Date So Recreate
	if (bModulesOutOfDate)
	{
		Modules.Empty();
		
		for (int i = 0; i < UIDs.Num(); i++)
		{
			auto Module = FFluidArchitectCoreProfileModule();
			Module.UID = UIDs[i];
			Module.ViewportName = ViewportNames[i];
			Module.ViewportType = ViewportTypes[i];
			Modules.Add(Module);	
		}
	}
}

void UFluidArchitectCoreProfile::RunModuleNodeGraph(const FFluidArchitectCoreProfileModule ProfileModule)
{
	//Runtime Data
	FFluidArchitectCoreGraphNode CurrentGraphNode;
	TArray<FFluidArchitectCoreGraphNode> ChildrenGraphNodes;
	
	//Find Root Node
	CurrentGraphNode.GraphNodeEvent = nullptr;
	for (int i = 0; i < ProfileModule.Nodes.Num(); i++)
		if (ProfileModule.Nodes[i].GraphNodeEvent != nullptr && ProfileModule.Nodes[i].GraphNodeEvent->IsA(UFluidArchitectCoreGraphNodeEventRoot::StaticClass()))
			CurrentGraphNode = ProfileModule.Nodes[i];
	
	//Do Current Node Event | Go To Next Node
	while(CurrentGraphNode.GraphNodeEvent != nullptr)
	{
		//Do Current Node Event | Set It To Null
		CurrentGraphNode.GraphNodeEvent->Action(this);

		//Clear Children Data
		ChildrenGraphNodes.Empty();
		
		//Find Children Nodes
		for (int i = 0; i < ProfileModule.NodeConnections.Num(); i++)
			if (ProfileModule.NodeConnections[i].ConnectionByUIDFrom == CurrentGraphNode.Guid)
			{
				for (int j= 0; j < ProfileModule.Nodes.Num(); j++)
					if (ProfileModule.Nodes[j].Guid == ProfileModule.NodeConnections[i].ConnectionByUIDTo)
						ChildrenGraphNodes.Add(ProfileModule.Nodes[j]);
			}

		//Quit Since We Have No Children Nodes
		if (ChildrenGraphNodes.Num() == 0)
			break;
		
		//Sort Child Data by Y Position
		Algo::SortBy(ChildrenGraphNodes, &FFluidArchitectCoreGraphNode::PosY, TGreater<>());
		Algo::Reverse( ChildrenGraphNodes);

		// Set Child Node
		if (CurrentGraphNode.GraphNodeEvent->IsA(UFluidArchitectCoreGraphNodeEventRandom::StaticClass()))
			CurrentGraphNode = ChildrenGraphNodes[SeedStream.RandRange(0,ChildrenGraphNodes.Num() - 1)];
		else
			CurrentGraphNode = ChildrenGraphNodes[0];

		//Transit
		if (CurrentGraphNode.GraphNodeEvent->IsA(UFluidArchitectCoreGraphNodeEventTransitTo::StaticClass()))
		{
			FText TransitToID = ((UFluidArchitectCoreGraphNodeEventTransitTo*)CurrentGraphNode.GraphNodeEvent)->ID;
			
			CurrentGraphNode.GraphNodeEvent = nullptr;
			for (int i = 0; i < ProfileModule.Nodes.Num(); i++)
				if (ProfileModule.Nodes[i].GraphNodeEvent != nullptr && ProfileModule.Nodes[i].GraphNodeEvent->IsA(UFluidArchitectCoreGraphNodeEventTransitFrom::StaticClass()))
					if (((UFluidArchitectCoreGraphNodeEventTransitFrom*)ProfileModule.Nodes[i].GraphNodeEvent)->ID.ToString() == TransitToID.ToString())
					{
						CurrentGraphNode = ProfileModule.Nodes[i];
						break;
					}
		}
	}
}

void UFluidArchitectCoreProfile::CreateSeedStream()
{
	if (ForcedSeed > 0) SeedStream = FRandomStream(ForcedSeed);
	else SeedStream.GenerateNewSeed();
}

void UFluidArchitectCoreProfile::SetForcedSeed(int Seed)
{
	ForcedSeed = Seed;
}
