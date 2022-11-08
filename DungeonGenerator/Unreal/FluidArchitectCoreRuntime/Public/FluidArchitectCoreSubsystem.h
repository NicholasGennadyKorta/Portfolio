// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidArchitectCoreRuntime.h"
#include "CoreMinimal.h"
#include "Misc/Guid.h"
#include "Math/RandomStream.h"
#include "FluidArchitectCoreSubsystem.generated.h"

//Fluid Architect Core Graph Node Event
UCLASS(abstract, EditInlineNew)
class FLUIDARCHITECTCORERUNTIME_API UFluidArchitectCoreGraphNodeEvent : public UObject
{
	GENERATED_BODY()

public:
	UFluidArchitectCoreGraphNodeEvent(){};
	virtual void Action(class UFluidArchitectCoreProfile* Profile) {};
	virtual int PinMode() {return 0;};
	virtual FLinearColor GetColor() {return  FLinearColor::Black;};
	virtual FText GetTitle() {return  FText::FromString("NULL");};
	virtual FText GetSearchName() {return  FText::FromString("NULL");};
};

//Fluid Architect Core Graph Node Event Transit From
UCLASS()
class FLUIDARCHITECTCORERUNTIME_API UFluidArchitectCoreGraphNodeEventRoot : public UFluidArchitectCoreGraphNodeEvent
{
	GENERATED_BODY()

	public:
	UFluidArchitectCoreGraphNodeEventRoot(){};
	void Action(class UFluidArchitectCoreProfile* Profile) override {};
	int PinMode() override {return 0;};
	FLinearColor GetColor() override {return  FLinearColor::Black;};
	FText GetTitle() override {return  FText::FromString("Core:Root");}
	FText GetSearchName() override {return  FText::FromString("Core:Root");}
};

//Fluid Architect Core Graph Node Event Transit To
UCLASS()
class FLUIDARCHITECTCORERUNTIME_API UFluidArchitectCoreGraphNodeEventTransitTo : public UFluidArchitectCoreGraphNodeEvent
{
	GENERATED_BODY()

public:
	UFluidArchitectCoreGraphNodeEventTransitTo(){};
	void Action(class UFluidArchitectCoreProfile* Profile) override {};
	int PinMode() override {return 1;};
	FLinearColor GetColor() override {return  FLinearColor::Black;};
	FText GetTitle() override {return  FText::FromString("Core:Transit To");}
	FText GetSearchName() override {return  FText::FromString("Core:Transit To");}

	UPROPERTY(EditDefaultsOnly,Category="Properties")
		FText ID;
};

//Fluid Architect Core Graph Node Event Transit From
UCLASS()
class FLUIDARCHITECTCORERUNTIME_API UFluidArchitectCoreGraphNodeEventTransitFrom : public UFluidArchitectCoreGraphNodeEvent
{
	GENERATED_BODY()

public:
	UFluidArchitectCoreGraphNodeEventTransitFrom(){};
	void Action(class UFluidArchitectCoreProfile* Profile) override {};
	int PinMode() override {return 1;};
	FLinearColor GetColor() override {return  FLinearColor::Black;};
	FText GetTitle() override {return  FText::FromString("Core:Transit From");}
	FText GetSearchName() override {return  FText::FromString("Core:Transit From");}

	UPROPERTY(EditDefaultsOnly,Category="Properties")
		FText ID;
};

//Fluid Architect Core Graph Node Event Transit From
UCLASS()
class FLUIDARCHITECTCORERUNTIME_API UFluidArchitectCoreGraphNodeEventRandom : public UFluidArchitectCoreGraphNodeEvent
{
	GENERATED_BODY()

public:
	UFluidArchitectCoreGraphNodeEventRandom(){};
	void Action(class UFluidArchitectCoreProfile* Profile) override {};
	int PinMode() override {return 1;};
	FLinearColor GetColor() override {return  FLinearColor::Black;};
	FText GetTitle() override {return  FText::FromString("Core:Random");}
	FText GetSearchName() override {return  FText::FromString("Core:Random");}	
};


//Fluid Architect Core Graph Node
USTRUCT()
struct FLUIDARCHITECTCORERUNTIME_API FFluidArchitectCoreGraphNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FGuid Guid;
	UPROPERTY()
		float PosX;
	UPROPERTY()
		float PosY;
	UPROPERTY(Instanced, EditDefaultsOnly)
		UFluidArchitectCoreGraphNodeEvent* GraphNodeEvent;	
};

//Fluid Architect Core Graph Node Connection
USTRUCT()
struct FLUIDARCHITECTCORERUNTIME_API FFluidArchitectCoreGraphNodeConnection
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
		FGuid ConnectionByUIDTo;
	UPROPERTY(EditDefaultsOnly)
		FGuid ConnectionByUIDFrom;
};

//Fluid Architect Core Profile Module
USTRUCT()
struct FLUIDARCHITECTCORERUNTIME_API FFluidArchitectCoreProfileModule
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
		FName UID;
	UPROPERTY(EditDefaultsOnly)
		FName ViewportName;
	UPROPERTY(EditDefaultsOnly)
		FName ViewportType;
	UPROPERTY(EditDefaultsOnly)
		TArray<FFluidArchitectCoreGraphNode> Nodes;
	UPROPERTY(EditDefaultsOnly)
		TArray<FFluidArchitectCoreGraphNodeConnection> NodeConnections;	
};

//Fluid Architect Core Profile
UCLASS()
class FLUIDARCHITECTCORERUNTIME_API UFluidArchitectCoreProfile : public UObject
{
	GENERATED_BODY()

public:

	virtual void Initialize() {};
	virtual void Generate() {};
	virtual void OnPreviewViewport(FName ModuleID, UWorld* World) {};

	void CreateModules(TArray<FName> UIDs, TArray<FName> ViewportNames, TArray<FName> ViewportTypes);
	void RunModuleNodeGraph(const FFluidArchitectCoreProfileModule ProfileModule);
	void CreateSeedStream();
	void SetForcedSeed(int Seed);
	
	UPROPERTY()
		TArray<FFluidArchitectCoreProfileModule> Modules;
	UPROPERTY(EditDefaultsOnly,Category="Properties")
		int ForcedSeed;
	FRandomStream SeedStream;
	TArray<TSubclassOf<UFluidArchitectCoreGraphNodeEvent>> RegistredGraphNodeEventClasses;
};