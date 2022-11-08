// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FluidEntitiesCharacterHybridData.generated.h"

//Fluid Entities Character Hybrid Transform Component
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridTransformComponent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
		FVector Location;
	UPROPERTY(BlueprintReadWrite)
		FVector ForwardVector;
	UPROPERTY(BlueprintReadWrite)
		FVector RightVector;
	UPROPERTY(BlueprintReadWrite)
		FRotator Rotation;
};

//Fluid Entities Character Hybrid Input XY Component
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridInputXYComponent
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadOnly)
		float X;
	UPROPERTY(BlueprintReadOnly)
		float Y;
	UPROPERTY(BlueprintReadOnly)
		float XY;
	UPROPERTY(BlueprintReadOnly)
		float XYPrevious;
};

//Fluid Entities Character Hybrid Input Yaw Component
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridInputYawComponent
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadOnly)
		float YawDeltaStart;
	UPROPERTY(BlueprintReadOnly)
		float YawDelta;
	UPROPERTY(BlueprintReadOnly)
		float YawStart;
	UPROPERTY(BlueprintReadOnly)
		float Yaw;
	//UPROPERTY(BlueprintReadOnly)
	//	float YawDot;
};

//Fluid Entities Character Hybrid Input State
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridInputStateComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		int LocalPlayerIndex;
	UPROPERTY(BlueprintReadWrite)
		bool bDisabled;
	
	UPROPERTY(BlueprintReadOnly)
		bool bDoesHaveInput;
	UPROPERTY(BlueprintReadOnly)
		bool bJustStartedInput;
	UPROPERTY(BlueprintReadOnly)
		bool bJustStoppedInput;
	UPROPERTY(BlueprintReadOnly)
		bool bJustChangedInput;
};

//Fluid Entities Character Hybrid Distance Matching Component
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridDistanceMatchingComponent
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite)
		FVector StartLocation;
	UPROPERTY(BlueprintReadWrite)
		FVector StopLocation;
	UPROPERTY(BlueprintReadWrite)
		FVector InputLocation;
	UPROPERTY(BlueprintReadWrite)
		FVector InputVelocity;
	UPROPERTY(BlueprintReadWrite)
		float InputBrakingDeceleration;
	UPROPERTY(BlueprintReadWrite)
		int Type;
};

//Fluid Entities Character Hybrid Orientation Warping Component
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridOrientationWarpingComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		float AngleMaxStepDelta;
	UPROPERTY(BlueprintReadWrite)
		float InterpSpeed;
	UPROPERTY(BlueprintReadWrite)
		int Type;
	
	UPROPERTY(BlueprintReadOnly)
		float Angle;
	UPROPERTY(BlueprintReadOnly)
        int CardinalDirection;
};

//Fluid Entities Character Hybrid Movement Component
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		FVector Direction;
	UPROPERTY(BlueprintReadOnly)
		FVector Velocity;
	UPROPERTY(BlueprintReadOnly)
		float VelocityRate;
	UPROPERTY(BlueprintReadOnly)
		float VelocitySize;
	UPROPERTY(BlueprintReadOnly)
		float LeanLeftRight;
	UPROPERTY(BlueprintReadOnly)
		bool bIsFalling;
	
	UPROPERTY(BlueprintReadWrite)
		float VelocityMaxWanted;
	UPROPERTY(BlueprintReadWrite)
		float Gait;
	UPROPERTY(BlueprintReadWrite)
		bool bStrafe;
};