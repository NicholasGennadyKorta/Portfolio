// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "GameFramework/WorldSettings.h"
#include "Animation/AnimInstanceProxy.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "FluidEntitiesCharacterHybridBlueprintLibrary.h"
#include "FluidEntitiesCharacterHybridAnimNodes.generated.h"

//Fluid Entities Character Hybrid Forward Declarations
class USkeletalMeshComponent;

//Fluid Entities Character Hybrid Anim Node Data
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridAnimNodeDataStridePivot
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Settings)
		FBoneReference Root;
	
	float CurrentDirection;
	FTransform Transform;

public:
	void Initialize(const FBoneContainer& Bones);
	bool IsValid(const FBoneContainer& Bones);
	FQuat GetDirectionRotation(float Direction, const float DeltaTime);
};

USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridAnimNodeDataStrideHipAdjustment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Settings)
		FBoneReference Hips;
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float AdjustmentRatio;
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = -1.0f))
		float MaxRecoveryRate;

public:
	void Initialize(const FBoneContainer& Bones);
	bool IsValid(const FBoneContainer& Bones);
};

/** The data structure to define a TwoBone IK Limb for stride warping */
USTRUCT(BlueprintType)
struct FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = LimbSettings)
		FBoneReference FKBone;
	UPROPERTY(EditAnywhere, Category = LimbSettings)
		FBoneReference IKBone;
	UPROPERTY(EditAnywhere, Category = LimbSettings, meta = (ClampMin = 2))
		int32 BoneCount;


	TArray<FBoneReference> Bones;
	float Length;
	float HeightDelta;
	FVector FKBoneLocation;
	FTransform FKBaseBoneTransform;

public:
	void Initialize(const FBoneContainer& PoseBones);
	bool IsValid(const FBoneContainer& PoseBones);
	void CalculateLength(FCSPose<FCompactPose>& Pose);
};

//Fluid Entities Character Hybrid Anim Node Stride Warping
USTRUCT(BlueprintInternalUseOnly)
struct FLUIDENTITIESCHARACTERHYBRIDRUNTIME_API FFluidEntitiesCharacterHybridAnimNodeStrideWarping : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Inputs, meta = (PinHiddenByDefault, ClampMin = 0.0f))
		float StrideScale;
	UPROPERTY(EditAnywhere, Category = Inputs, meta = (PinHiddenByDefault, ClampMin = -180.0f, ClampMax = 180.0f))
		float StrideAngle;
	
	UPROPERTY(EditAnywhere, Category = Bones)
		FFluidEntitiesCharacterHybridAnimNodeDataStridePivot Pivot;
	UPROPERTY(EditAnywhere, Category = Bones)
		FFluidEntitiesCharacterHybridAnimNodeDataStrideHipAdjustment HipAdjustment;
	UPROPERTY(EditAnywhere, Category = Bones)
		TArray<FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition> Limbs;

	
	FAnimInstanceProxy* AnimInstanceProxy;
	float LastHipShift;
	float DeltaTime;
	bool bValidCheckResult;
	
public:
	void EvaluateStride(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms);
	bool CheckValidBones(const FBoneContainer& RequiredBones);
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	virtual bool HasPreUpdate() const override;
	virtual void PreUpdate(const UAnimInstance* InAnimInstance) override;
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;

};