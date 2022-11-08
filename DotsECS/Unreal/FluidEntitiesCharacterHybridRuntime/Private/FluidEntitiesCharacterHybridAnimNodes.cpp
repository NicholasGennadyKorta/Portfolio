// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidEntitiesCharacterHybridAnimNodes.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Anim Node Data

void FFluidEntitiesCharacterHybridAnimNodeDataStridePivot::Initialize(const FBoneContainer& Bones)
{
	Root.Initialize(Bones);
}

bool FFluidEntitiesCharacterHybridAnimNodeDataStridePivot::IsValid(const FBoneContainer & Bones)
{
	return Root.IsValidToEvaluate(Bones);
}

FQuat FFluidEntitiesCharacterHybridAnimNodeDataStridePivot::GetDirectionRotation(float Direction, const float DeltaTime)
{
	CurrentDirection = UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleWrap(Direction);
	return FQuat(FVector::UpVector, FMath::DegreesToRadians(CurrentDirection));
}

void FFluidEntitiesCharacterHybridAnimNodeDataStrideHipAdjustment::Initialize(const FBoneContainer& Bones)
{
	Hips.Initialize(Bones);
}

bool FFluidEntitiesCharacterHybridAnimNodeDataStrideHipAdjustment::IsValid(const FBoneContainer& Bones)
{
	return Hips.IsValidToEvaluate(Bones);
}

void FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition::Initialize(const FBoneContainer& PoseBones)
{
	Length = -1.0f;

	//Initialize set bones
	FKBone.Initialize(PoseBones);
	IKBone.Initialize(PoseBones);

	Bones.Empty(BoneCount);

	if (!FKBone.IsValidToEvaluate(PoseBones))
		return;

	const FReferenceSkeleton& RefSkeleton = PoseBones.GetReferenceSkeleton();

	int32 ParentBoneIndex = PoseBones.GetParentBoneIndex(FKBone.BoneIndex);
	for (int32 i = 0; i < BoneCount; ++i)
	{
		if(ParentBoneIndex == 0)
		{
			break;
		}

		FBoneReference Bone = FBoneReference(RefSkeleton.GetBoneName(ParentBoneIndex));
		Bone.Initialize(PoseBones);
		Bones.Add(Bone);

		ParentBoneIndex = PoseBones.GetParentBoneIndex(ParentBoneIndex);
	}
}

bool FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition::IsValid(const FBoneContainer& PoseBones)
{
	bool bIsValid = (IKBone.IsValidToEvaluate(PoseBones)
		&& FKBone.IsValidToEvaluate(PoseBones))
		&& (Bones.Num() == BoneCount)
		&& (BoneCount > 1);

	for(FBoneReference& Bone : Bones)
	{
		if (!Bone.IsValidToEvaluate(PoseBones))
		{
			bIsValid = false;
			break;
		}
	}

	return bIsValid;
}

void FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition::CalculateLength(FCSPose<FCompactPose>& Pose)
{
	//Length Of Leg Bones
	Length = Pose.GetLocalSpaceTransform(FKBone.CachedCompactPoseIndex).GetLocation().Size();
	for (int32 i = 0; i < BoneCount -1; ++i)
		Length += Pose.GetLocalSpaceTransform(Bones[i].CachedCompactPoseIndex).GetLocation().Size();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Anim Node Stride Warping

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms)
{
	EvaluateStride(Output, OutBoneTransforms);
	OutBoneTransforms.Sort(FCompareBoneTransformIndex());

	if (Alpha < 1.0f)
		Output.Pose.LocalBlendCSBoneTransforms(OutBoneTransforms, Alpha);
}

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::EvaluateStride(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	//Stride Pivot
	const FTransform StrideRootTransform_CS = Output.Pose.GetComponentSpaceTransform(Pivot.Root.CachedCompactPoseIndex);
	FTransform HipTransform_CS = Output.Pose.GetComponentSpaceTransform(HipAdjustment.Hips.CachedCompactPoseIndex);

	Pivot.Transform.SetRotation(Pivot.GetDirectionRotation(StrideAngle, DeltaTime));
	Pivot.Transform.SetLocation(StrideRootTransform_CS.GetLocation());
	Pivot.Transform.AddToTranslation(Pivot.Transform.TransformPosition(FVector(0.0f, 0, 0.0f)));

	FTransform PivotTransform = Pivot.Transform;
	
	
	//Limps
	float HighestTipZDelta = - 1000000.0f;
	for (FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition & Limb : Limbs)
	{
		//Not Valid
		if (Limb.Bones.Num() < 2)
			continue;
		
		//Get Relevant transforms, rotations and locations
		Limb.FKBaseBoneTransform = Output.Pose.GetComponentSpaceTransform(Limb.Bones.Last().CachedCompactPoseIndex);
		
		FVector BaseBoneLocation_CS = Limb.FKBaseBoneTransform.GetLocation();
		FVector TipLocation_CS = Output.Pose.GetComponentSpaceTransform(Limb.FKBone.CachedCompactPoseIndex).GetLocation();

		//Limb Length
		if (Limb.Length < 0.0f)
			Limb.CalculateLength(Output.Pose);
		
		
		//Scale foot position Y component value in the StridePivotSpace (SPS)
		float MaxLimbLength = (TipLocation_CS - BaseBoneLocation_CS).Size();
		MaxLimbLength += (Limb.Length - MaxLimbLength);

		FVector NewTipLocation_SPS = Pivot.Transform.InverseTransformPosition(TipLocation_CS);
		NewTipLocation_SPS.Y *= StrideScale;

		Limb.FKBoneLocation = PivotTransform.TransformPosition(NewTipLocation_SPS);

		//Push foot back towards hips to ensure legs are only ever compressed and not ever extended
		FVector newBaseToTipVector_CS = Limb.FKBoneLocation - BaseBoneLocation_CS;
		float CurrentLimbLength = newBaseToTipVector_CS.Size();
		if (CurrentLimbLength > MaxLimbLength)
		{
			Limb.FKBoneLocation -= newBaseToTipVector_CS.GetSafeNormal() * (CurrentLimbLength - MaxLimbLength);
		}

		//Check if this Limb has the lowest Z delta
		Limb.HeightDelta = Limb.FKBoneLocation.Z - TipLocation_CS.Z;

		if (Limb.HeightDelta > HighestTipZDelta)
			HighestTipZDelta = Limb.HeightDelta;
	}

	//Pull Hips Down
	float HipShift = -HighestTipZDelta * HipAdjustment.AdjustmentRatio;
	if (HipAdjustment.MaxRecoveryRate > 0.0f && HipShift > LastHipShift)
		LastHipShift = UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetFloatMoveToward(LastHipShift, HipShift, HipAdjustment.MaxRecoveryRate * DeltaTime);
	else
		LastHipShift = HipShift;
	
	const FVector HipAdjust = FVector(0.0f, 0.0f, LastHipShift);
	const FVector CurrentHipLocation_CS = HipTransform_CS.GetLocation();

	FVector NewHipLocation_CS = CurrentHipLocation_CS + HipAdjust;

	HipTransform_CS.SetLocation(NewHipLocation_CS);
	OutBoneTransforms.Add(FBoneTransform(HipAdjustment.Hips.CachedCompactPoseIndex, HipTransform_CS));

	//Apply Hip Adjustment To Limbs
	for (int32 i = 0; i < Limbs.Num(); ++i)
	{
		FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition& Limb = Limbs[i];

		FVector FootAdjust = HipAdjust + FVector(0.0f, 0.0f, ((HighestTipZDelta)-Limb.HeightDelta) * HipAdjustment.AdjustmentRatio + HipShift - LastHipShift);
		Limb.FKBoneLocation += FootAdjust;

		FTransform IkTransform_CS = Output.Pose.GetComponentSpaceTransform(Limb.FKBone.CachedCompactPoseIndex);
		IkTransform_CS.SetLocation(Limb.FKBoneLocation);

		OutBoneTransforms.Add(FBoneTransform(Limb.IKBone.CachedCompactPoseIndex, IkTransform_CS));
	}
}

bool FFluidEntitiesCharacterHybridAnimNodeStrideWarping::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return bValidCheckResult;
}

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	FAnimNode_SkeletalControlBase::Initialize_AnyThread(Context);
	AnimInstanceProxy = Context.AnimInstanceProxy;
}

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	FAnimNode_SkeletalControlBase::CacheBones_AnyThread(Context);
}

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::UpdateInternal(const FAnimationUpdateContext& Context)
{
	FAnimNode_SkeletalControlBase::UpdateInternal(Context);
	DeltaTime = Context.GetDeltaTime();
}

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::GatherDebugData(FNodeDebugData& DebugData)
{
	ComponentPose.GatherDebugData(DebugData);
}

bool FFluidEntitiesCharacterHybridAnimNodeStrideWarping::HasPreUpdate() const
{
	return false;
}

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::PreUpdate(const UAnimInstance* InAnimInstance)
{
}

void FFluidEntitiesCharacterHybridAnimNodeStrideWarping::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	HipAdjustment.Initialize(RequiredBones);
	Pivot.Initialize(RequiredBones);

	bValidCheckResult = true; 
	if (Limbs.Num() > 0)
	{
		for (FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition& LimbDef : Limbs)
		{
			LimbDef.Initialize(RequiredBones);

			if (!LimbDef.IsValid(RequiredBones))
				bValidCheckResult = false;
		}
	}
	else bValidCheckResult = false;


	if (!HipAdjustment.IsValid(RequiredBones) || !Pivot.IsValid(RequiredBones))
		bValidCheckResult = false;
}

bool FFluidEntitiesCharacterHybridAnimNodeStrideWarping::CheckValidBones(const FBoneContainer& RequiredBones)
{
	bValidCheckResult = HipAdjustment.IsValid(RequiredBones) && Pivot.IsValid(RequiredBones) && (Limbs.Num() > 0);

	for (FFluidEntitiesCharacterHybridAnimNodeDataLimbDefinition& LimbDef : Limbs)
		if (!LimbDef.IsValid(RequiredBones))
			bValidCheckResult = false;
	
	return bValidCheckResult;
}