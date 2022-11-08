// 

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimInstanceProxy.h"
#include "FluidEntitiesCharacterHybridBlueprintLibrary.generated.h"

UCLASS()
class FLUIDENTITIESCHARACTERHYBRIDRUNTIME_API UFluidEntitiesCharacterHybridBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//Fluid Entities Character Hybrid Blueprint Library Character
public:

	UFUNCTION(BlueprintCallable)
		static void SetCharacterViewMode(bool bIsThirdPerson, USkinnedMeshComponent* ThirdPersonFullBodyMeshComponent,USkinnedMeshComponent* FirstPersonFullBodyMeshComponent, USkinnedMeshComponent* FirstPersonLegsMeshComponent, USkinnedMeshComponent* FirstPersonArmsMeshComponent, bool& bIsThirdPerson_Out);
		
//Fluid Entities Character Hybrid Blueprint Library Math
public:
	
	static float MathGetAngleDelta(const float StartAngle, const float EndAngle);
	static float MathGetAngleWrap(float Angle);
	static float MathGetFloatMoveToward(const float Start, const float End, const float MaxDelta);
	static FVector MathGetBoneWorldLocation(const FTransform& InBoneTransform_CS, FAnimInstanceProxy* InAnimInstanceProxy);
	static FVector MathGetBoneWorldLocation(const FVector& InBoneLocation_CS, FAnimInstanceProxy* InAnimInstanceProxy);
};
