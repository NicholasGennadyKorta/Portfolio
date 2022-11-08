// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidArchitectIssacProfile.h"
#include "TraceServices/Public/TraceServices/Containers/Timelines.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Profile Main

void UFluidArchitectIssacProfile::Initialize()
{
	//Create Modules (Will Only Create If Not Already Created)
	const TArray<FName> UIDs = {"fluidArchitect-issac-execution-graph", "fluidArchitect-issac-data-viewport-data" };
	const TArray<FName> ViewportNames = {"Issac Execution Graph", "Data Viewport" };
	const TArray<FName> ViewportTypes = {"graphEditor","previewViewport" };;
	CreateModules(UIDs,ViewportNames,ViewportTypes);

	//Register Graph Node Events
	RegistredGraphNodeEventClasses.Empty();
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventRoot::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventTransitTo::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventTransitFrom::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectCoreGraphNodeEventRandom::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectIssacGraphNodeEventGenerateRooms::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectIssacGraphNodeEventPostProcessingStartRoom::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectIssacGraphNodeEventPostProcessingEndRooms::StaticClass());
	RegistredGraphNodeEventClasses.Add(UFluidArchitectIssacGraphNodeEventPostProcessingSecretRooms::StaticClass());
}

void UFluidArchitectIssacProfile::Generate()
{
	//Create Seed
	CreateSeedStream();
	
	//Run Our Execution Graph 
	for (int i = 0; i < Modules.Num(); i++)
		if (Modules[i].UID == "fluidArchitect-issac-execution-graph")
			RunModuleNodeGraph(Modules[i]);
}

void UFluidArchitectIssacProfile::OnPreviewViewport(FName ModuleID, UWorld* World)
{
	if (ModuleID != "fluidArchitect-issac-data-viewport-data")
		return;

	FColor RoomColor = FColor::White;
	for (int i = 0; i < Rooms.Num(); i++)
	{
		//Get Room Color
		RoomColor = Rooms[i].Stamps.Contains("end-room") ? FColor::Magenta : FColor::White;
		if (Rooms[i].Stamps.Contains("secret-room")) RoomColor = FColor::FromHex("#202020");
		
		//Draw Room
		DrawDebugBox(World,FVector(100 * Rooms[i].CellPosition.X,100 * Rooms[i].CellPosition.Y,0),FVector(40,40,0.1),RoomColor,false,0,0,2.5f);	

		//Draw Start Room
		if (Rooms[i].Stamps.Contains("start-room"))
			DrawDebugBox(World,FVector(100 * Rooms[i].CellPosition.X,100 * Rooms[i].CellPosition.Y,0),FVector(30,30,0.1),FColor::Green,false,0,0,2.5f);	

		//Draw Neighbor Room Lines
		for (int j = 0; j < Rooms[i].NeighborCellPositions.Num(); j++)
			DrawDebugLine(World,FVector(100 * Rooms[i].CellPosition.X,100 * Rooms[i].CellPosition.Y,0),FVector(100 * Rooms[i].NeighborCellPositions[j].X,100 * Rooms[i].NeighborCellPositions[j].Y,0),FColor::Cyan);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Profile Room

void UFluidArchitectIssacProfile::RoomExpand(FIntVector CurrentCellPosition,float NorthChance, float EastChance, float SouthChance, float WestChance,bool bOnlyExpandOnce)
{
	TArray<FIntVector> Directions = { FIntVector(0,1,0), FIntVector(0,-1,0), FIntVector(-1,0,0) ,FIntVector(1,0,0)};
	TArray<float> Chances = { NorthChance,SouthChance,WestChance,EastChance };

	//Expand
	while(Directions.Num() > 0)
	{
		int i = SeedStream.RandRange(0,Directions.Num() - 1);
		if (SeedStream.RandRange(0.0f,1.0f) <= Chances[i] && !ClosedSet.Contains(CurrentCellPosition + Directions[i]))
			RoomAdd(CurrentCellPosition,Directions[i]);

		Directions.RemoveAt(i);
		Chances.RemoveAt(i);

		if (bOnlyExpandOnce)
			break;
	}
}

void UFluidArchitectIssacProfile::RoomAdd(FIntVector CurrentCellPosition, FIntVector CurrentCellPositionStep)
{
	//Create Room
	auto NewRoom = FFluidArchitectIssacRoom();

	//Link Rooms Together
	if (ClosedSet.Num() != 0)
	{
		NewRoom.NeighborCellPositions.Add(CurrentCellPosition);
		NewRoom.CellPosition = CurrentCellPosition + CurrentCellPositionStep;
		Rooms[RoomIndexs[CurrentCellPosition]].NeighborCellPositions.Add(NewRoom.CellPosition);
	}
	
	//First Room So No Linking
	else
		NewRoom.CellPosition = FIntVector::ZeroValue;
	
	//Add To Data
	RoomIndexs.Add(NewRoom.CellPosition,RoomIndexs.Num());
	Rooms.Add(NewRoom);
	OpenSet.Add(NewRoom.CellPosition);
	ClosedSet.Add(NewRoom.CellPosition);
}

void UFluidArchitectIssacProfile::RoomRemove(FIntVector CellPosition)
{
	//Find Room Index And Room
	int RoomIndex = -1;
	FFluidArchitectIssacRoom Room;
	for (int i = 0; i < Rooms.Num(); i++)
		if (Rooms[i].CellPosition == CellPosition)
		{
			RoomIndex = i;
			Room = Rooms[i];
		}

	//Not A Valid Room So Return
	if (RoomIndex == -1) return;

	//Remove Room
	Rooms.RemoveAt(RoomIndex);

	//Remove All Room Neighbors That Are This Rooms Neighbor
	for (int i = 0; i < Rooms.Num(); i++)
		for (int j = Rooms[i].NeighborCellPositions.Num(); j--;)
		{
			if (Rooms[i].NeighborCellPositions[j] == CellPosition)
				Rooms[i].NeighborCellPositions.RemoveAt(j);
		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Graph Node Event Generate Rooms

void UFluidArchitectIssacGraphNodeEventGenerateRooms::Action(UFluidArchitectCoreProfile* Profile)
{
	//Cast To Issac Profile
	UFluidArchitectIssacProfile* IssacProfile = (UFluidArchitectIssacProfile*)Profile;
	if (!IssacProfile)
		return;

	//Reset Data
    IssacProfile->Rooms.Empty();
    IssacProfile->RoomIndexs.Empty();
    IssacProfile->OpenSet.Empty();
    IssacProfile->ClosedSet.Empty();

	//Make Sure Min Room Amount Is Not More Than Max Room Amount
	MinStepAmount = FMath::Clamp(MinStepAmount,0,MaxStepAmount);

	//Create First Room
	IssacProfile->RoomAdd(FIntVector::ZeroValue,FIntVector::ZeroValue);

	//Generate Rooms If We Have Rooms In Open Set And We Have Not Reached Max Amount
	while(IssacProfile->OpenSet.Num() != 0 && IssacProfile->ClosedSet.Num() < MaxStepAmount)
	{
		IssacProfile->RoomExpand(IssacProfile->OpenSet[0],StepNorthRate,StepEastRate,StepSouthRate,StepWestRate,false);
		IssacProfile->OpenSet.RemoveAt(0);
	}

	//Not Enough Rooms, Try Again
	if (IssacProfile->Rooms.Num() < MinStepAmount)
		Action(Profile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Graph Node Event Post Processing Start Room

void UFluidArchitectIssacGraphNodeEventPostProcessingStartRoom::Action(UFluidArchitectCoreProfile* Profile)
{
	//Cast To Issac Profile
	UFluidArchitectIssacProfile* IssacProfile = (UFluidArchitectIssacProfile*)Profile;
	if (!IssacProfile)
		return;

	//Remove Start Room If Exists
	for (int i = 0; i < IssacProfile->Rooms.Num(); i++)
		if (IssacProfile->Rooms[i].NeighborCellPositions.Num() <= 1)
			if (IssacProfile->Rooms[i].Stamps.Contains("start-room"))
			{
				IssacProfile->Rooms[i].Stamps.Remove("start-room");
				break;
			}
	
	//Get Random Start Room (Exclude End Rooms And Secret Rooms)
	while (true)
	{
		//Get Room
		auto Room = IssacProfile->Rooms[IssacProfile->SeedStream.RandRange(0,IssacProfile->Rooms.Num() - 1)];
		if (Room.Stamps.Contains("end-room") || Room.Stamps.Contains("secret-room"))
			continue;

		//Stamp As Start Room
		IssacProfile->Rooms[IssacProfile->SeedStream.RandRange(0,IssacProfile->Rooms.Num() - 1)].Stamps.Add("start-room");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Graph Node Event Post Processing End Rooms

void UFluidArchitectIssacGraphNodeEventPostProcessingEndRooms::Action(UFluidArchitectCoreProfile* Profile)
{
	//Cast To Issac Profile
	UFluidArchitectIssacProfile* IssacProfile = (UFluidArchitectIssacProfile*)Profile;
	if (!IssacProfile)
		return;

	//Get End Room Wanted Amount
	int EndRoomAmountWantedAmount = Profile->SeedStream.RandRange(MinEndRoomAmount,MaxEndRoomAmount);
	TArray<int> EndRoomIndexs;

	//Make Or Delete End Rooms
	while (true)
	{
		//Get End Rooms For This Step (Exclude Start Room)
		EndRoomIndexs.Empty();
		for (int i = 0; i < IssacProfile->Rooms.Num(); i++)
			if (IssacProfile->Rooms[i].NeighborCellPositions.Num() <= 1 && !IssacProfile->Rooms[i].Stamps.Contains("start-room"))
				EndRoomIndexs.Add(i);

		//To Many Rooms So Delete One
		if (EndRoomIndexs.Num() > EndRoomAmountWantedAmount)
			IssacProfile->RoomRemove(IssacProfile->Rooms[EndRoomIndexs[0]].CellPosition);
		else break;
	}
	
	//Stamp End Rooms | Remove Secret Room Stamp If Applied (Exclude Start Room)
	for (int i = 0; i < IssacProfile->Rooms.Num(); i++)
		if (IssacProfile->Rooms[i].NeighborCellPositions.Num() <= 1 && !IssacProfile->Rooms[i].Stamps.Contains("start-room"))
		{
			IssacProfile->Rooms[i].Stamps.Add("end-room");
			IssacProfile->Rooms[i].Stamps.Remove("secret-room");
		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect Issac Graph Node Event Post Processing Secret Rooms

void UFluidArchitectIssacGraphNodeEventPostProcessingSecretRooms::Action(UFluidArchitectCoreProfile* Profile)
{
	//Cast To Issac Profile
	UFluidArchitectIssacProfile* IssacProfile = (UFluidArchitectIssacProfile*)Profile;
	if (!IssacProfile)
		return;
	
	//Add Array Of Possible Rooms
	TArray<FFluidArchitectIssacRoom*> Rooms;
	for (int i = 0; i < IssacProfile->Rooms.Num() - 1; i++)
		if (IssacProfile->Rooms[i].NeighborCellPositions.Num() == 1 && !IssacProfile->Rooms[i].Stamps.Contains("secret-room") && !IssacProfile->Rooms[i].Stamps.Contains("start-room"))
			Rooms.Add(&IssacProfile->Rooms[i]);

	//Shuffle Rooms Array
	for (int32 i = Rooms.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::FloorToInt(IssacProfile->SeedStream.FRand() * (i + 1)) % Rooms.Num();
		auto temp = Rooms[i];
		Rooms[i] = Rooms[j];
		Rooms[j] = temp;
	}

	//Set Secret Rooms
	for (int i = 0; i < Amount; i++)
	{
		Rooms[0]->Stamps.Add("secret-room");
		Rooms.RemoveAt(0);
	}
}