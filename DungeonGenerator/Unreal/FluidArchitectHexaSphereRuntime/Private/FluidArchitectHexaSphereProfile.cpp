// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidArchitectHexaSphereProfile.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Hexasphere Profile Main

void UFluidArchitectHexaSphereProfile::Initialize()
{
	//Create Modules (Will Only Create If Not Already Created)
	const TArray<FName> UIDs = {"fluidArchitect-hexaShere-execution-graph", "fluidArchitect-hexaSphere-viewport" };
	const TArray<FName> ViewportNames = {"HexaSphere Execution Graph", "Viewport" };
	const TArray<FName> ViewportTypes = {"graphEditor","previewViewport" };;
	CreateModules(UIDs,ViewportNames,ViewportTypes);

	//Register Graph Node Events
	RegistredGraphNodeEventClasses.Empty();
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventRoot::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventTransitTo::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventTransitFrom::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventRandom::StaticClass());
}

void UFluidArchitectHexaSphereProfile::Generate()
{
	//Create Seed
	CreateSeedStream();
	
	//Run Our Execution Graph 
	for (int i = 0; i < Modules.Num(); i++)
		if (Modules[i].UID == "fluidArchitect-hexaShere-execution-graph")
			RunModuleNodeGraph(Modules[i]);
}

void UFluidArchitectHexaSphereProfile::OnPreviewViewport(FName ModuleID, UWorld* World)
{
	if (ModuleID != "fluidArchitect-hexaSphere-viewport")
		return;
}