// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DrawDebugHelpers.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FluidArchitectCoreSubsystem.h"
#include "FluidArchitectIssacProfile.generated.h"

//Fluid Architect Issac Room
struct FFluidArchitectIssacRoom
{
	FIntVector CellPosition;
	TArray<FIntVector> NeighborCellPositions;
	TArray<FName> Stamps;
};

//Fluid Architect Issac Profile
UCLASS(BlueprintType)
class FLUIDARCHITECTISSACRUNTIME_API UFluidArchitectIssacProfile : public UFluidArchitectCoreProfile
{
	GENERATED_BODY()

//Main	
public:
	
	void Initialize() override;
	void Generate() override;
	void OnPreviewViewport(FName ModuleID, UWorld* World) override;

//Room
public:
	
	void RoomExpand(FIntVector CurrentCellPosition,float NorthChance, float EastChance, float SouthChance, float WestChance, bool bOnlyExpandOnce);
	void RoomAdd(FIntVector CurrentCellPosition, FIntVector CurrentCellPositionStep);
	void RoomRemove(FIntVector CellPosition);
	
	TArray<FIntVector> OpenSet;
	TArray<FIntVector> ClosedSet;
	TArray<FFluidArchitectIssacRoom> Rooms;
	TMap<FIntVector,int> RoomIndexs;
};

//Fluid Architect Issac Graph Node Event Core
UCLASS()
class FLUIDARCHITECTISSACRUNTIME_API UFluidArchitectIssacGraphNodeEventCore : public UFluidArchitectCoreGraphNodeEvent
{
	GENERATED_BODY()

public:
	UFluidArchitectIssacGraphNodeEventCore(){};
	void Action(UFluidArchitectCoreProfile* Profile){};
	int PinMode() {return 1;};
	FLinearColor GetColor() {return FLinearColor::Black; };
	FText GetTitle() {return  FText::FromString("Issac:Core");};
	FText GetSearchName() override {return  FText::FromString("Issac:Core");}
};

//Fluid Architect Issac Graph Node Event Generate Rooms
UCLASS()
class FLUIDARCHITECTISSACRUNTIME_API UFluidArchitectIssacGraphNodeEventGenerateRooms : public UFluidArchitectIssacGraphNodeEventCore
{
	GENERATED_BODY()

public:
	UFluidArchitectIssacGraphNodeEventGenerateRooms(){};
	void Action(UFluidArchitectCoreProfile* Profile) override;
	int PinMode() override {return 1;};
	FLinearColor GetColor() override {return FLinearColor::FromSRGBColor(FColor(23,73,58,255)); };
	FText GetTitle() override {return  FText::FromString("Issac:Generate Rooms");};
	FText GetSearchName() override {return  FText::FromString("Issac:Generate Rooms");};

	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="2",ClampMax="100"),Category="Step Amount")
		int MinStepAmount;
	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="2",ClampMax="100"),Category="Step Amount")
		int MaxStepAmount;
	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="0",ClampMax="1"),Category="Step Expand Rates")
		float StepNorthRate;
	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="0",ClampMax="1"),Category="Step Expand Rates")
		float StepEastRate;
	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="0",ClampMax="1"),Category="Step Expand Rates")
		float StepSouthRate;
	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="0",ClampMax="1"),Category="Step Expand Rates")
		float StepWestRate;
};

//Fluid Architect Issac Graph Node Event Post Processing Start Room
UCLASS()
class FLUIDARCHITECTISSACRUNTIME_API UFluidArchitectIssacGraphNodeEventPostProcessingStartRoom : public UFluidArchitectIssacGraphNodeEventCore
{
	GENERATED_BODY()

public:
	UFluidArchitectIssacGraphNodeEventPostProcessingStartRoom(){};
	void Action(UFluidArchitectCoreProfile* Profile) override;
	int PinMode() override {return 1;};
	FLinearColor GetColor() override {return FLinearColor::FromSRGBColor(FColor(23,67,80,255)); };
	FText GetTitle() override {return  FText::FromString("Issac:Postprocessing Start Room");}
	FText GetSearchName() override {return  FText::FromString("Issac:Postprocessing Start Room");}
};

//Fluid Architect Issac Graph Node Event Post Processing End Rooms
UCLASS()
class FLUIDARCHITECTISSACRUNTIME_API UFluidArchitectIssacGraphNodeEventPostProcessingEndRooms : public UFluidArchitectIssacGraphNodeEventCore
{
	GENERATED_BODY()

public:
	UFluidArchitectIssacGraphNodeEventPostProcessingEndRooms(){};
	void Action(UFluidArchitectCoreProfile* Profile) override;
	int PinMode() override {return 1;};
	FLinearColor GetColor() override {return FLinearColor::FromSRGBColor(FColor(23,67,80,255)); };
	FText GetTitle() override {return  FText::FromString("Issac:Postprocessing End Rooms");};
	FText GetSearchName() override {return  FText::FromString("Issac:Postprocessing End Rooms");}

	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="0",ClampMax="100"),Category="Properties")
		int MinEndRoomAmount;
	UPROPERTY(EditDefaultsOnly,meta=(ClampMin="0",ClampMax="100"),Category="Properties")
		int MaxEndRoomAmount;
};

//Fluid Architect Issac Graph Node Event Post Processing End Rooms
UCLASS()
class FLUIDARCHITECTISSACRUNTIME_API UFluidArchitectIssacGraphNodeEventPostProcessingSecretRooms : public UFluidArchitectIssacGraphNodeEventCore
{
	GENERATED_BODY()

public:
	UFluidArchitectIssacGraphNodeEventPostProcessingSecretRooms(){};
	void Action(UFluidArchitectCoreProfile* Profile) override;
	int PinMode() override {return 1;};
	FLinearColor GetColor() override {return FLinearColor::FromSRGBColor(FColor(23,67,80,255)); };
	FText GetTitle() override {return  FText::FromString("Issac:Postprocessing Secret Rooms");};
	FText GetSearchName() override {return  FText::FromString("Issac:Postprocessing Secret Rooms");}

	UPROPERTY(EditDefaultsOnly,Category="Properties")
		int Amount;
};