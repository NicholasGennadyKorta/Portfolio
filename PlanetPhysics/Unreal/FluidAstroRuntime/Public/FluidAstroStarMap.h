// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "FluidAstroStarMap.generated.h"

//Fluid Astro Star Map Model HyqV3 Table Row
//http://www.astronexus.com/hyg
USTRUCT(BlueprintType)
struct FLUIDASTRORUNTIME_API FFluidAstroStarMapModelHyqV3TableRow  : public FTableRowBase
{
	GENERATED_BODY()
public:

	//Variable Names Match Here From Downloaded File //http://www.astronexus.com/hyg
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Proper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Con;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Spect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Z;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Dist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Mag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ColorIndex;
};

//Fluid Astro Star Map Model Base
UCLASS()
class FLUIDASTRORUNTIME_API UFluidAstroStarMapModelBase : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Data (Imported)")
		TArray<FText> StarNames;
	UPROPERTY(BlueprintReadOnly, Category = "Data (Imported)")
		TArray<FVector> StarPositions;
	UPROPERTY(BlueprintReadOnly, Category = "Data (Imported)")
		TArray<FVector> StarDirection;
	UPROPERTY(BlueprintReadOnly, Category = "Data (Imported)")
		TArray<float> StarDistancesInParsecs;
	UPROPERTY(BlueprintReadOnly, Category = "Data (Imported)")
		TArray<float> StarDistancesInLightYears;
	UPROPERTY(BlueprintReadOnly, Category = "Data (Imported)")
		TArray<float> StarMagnitudes;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Data (Imported)", meta = (EditCondition = false))
        int StarCount;
	

	virtual void LoadData() { };
};

//Fluid Astro Star Map Model HyqV3
//http://www.astronexus.com/hyg 
UCLASS()
class FLUIDASTRORUNTIME_API UFluidAstroStarMapModelHyqV3 : public UFluidAstroStarMapModelBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Data (To Load In)")
		UDataTable* StarMapModelHyqV3DataTable;

	virtual void LoadData() override;
	FVector LatitudeLongitudeToPoint(const float Latitude, const float Longitude, const float Radius) const;
};

//Fluid Astro Star Map Actor
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLUIDASTRORUNTIME_API AFluidAstroStarMapActor : public AActor
{
	GENERATED_BODY()

public:

	AFluidAstroStarMapActor();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		void GetStarRenderPositions(TArray<FVector>& RenderLocations_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Astro Star Map")
		void DrawDebugStars();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Fluid Astro Star Map")
		UFluidAstroStarMapModelBase* StarMapModel;
	UPROPERTY(EditAnywhere, Category = "Fluid Astro Star Map")
		float StarDistanceRenderSize;
	UPROPERTY(EditAnywhere, Category = "Fluid Astro Star Map")
		float StarRenderSize;
};
