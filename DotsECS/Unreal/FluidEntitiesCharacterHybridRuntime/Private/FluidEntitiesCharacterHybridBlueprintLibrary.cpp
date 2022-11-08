// 

#include "FluidEntitiesCharacterHybridBlueprintLibrary.h"

//////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Blueprint Library Character

void UFluidEntitiesCharacterHybridBlueprintLibrary::SetCharacterViewMode(bool bIsThirdPerson, USkinnedMeshComponent* ThirdPersonFullBodyMeshComponent, USkinnedMeshComponent* FirstPersonFullBodyMeshComponent, USkinnedMeshComponent* FirstPersonLegsMeshComponent, USkinnedMeshComponent* FirstPersonArmsMeshComponent, bool& bIsThirdPerson_Out)
{
	//Pass Third Person
	bIsThirdPerson_Out = bIsThirdPerson;
	
	//Not Valid Mesh Components
    if (ThirdPersonFullBodyMeshComponent == nullptr || FirstPersonFullBodyMeshComponent == nullptr || FirstPersonLegsMeshComponent == nullptr || FirstPersonArmsMeshComponent == nullptr)
    	return;

	//Get As Pawn
	auto AsPawn = (APawn*)ThirdPersonFullBodyMeshComponent->GetOwner();
	if (AsPawn == nullptr) return;

	//Set First Person | Third Person Visibility
	ThirdPersonFullBodyMeshComponent->SetVisibility(bIsThirdPerson);
	FirstPersonFullBodyMeshComponent->SetVisibility(!bIsThirdPerson);
	FirstPersonLegsMeshComponent->SetVisibility(!bIsThirdPerson);
	FirstPersonArmsMeshComponent->SetVisibility(!bIsThirdPerson);
	FirstPersonFullBodyMeshComponent->SetOwnerNoSee(true);
	FirstPersonLegsMeshComponent->HideBoneByName("spine_02",EPhysBodyOp::PBO_None);
	FirstPersonLegsMeshComponent->SetOnlyOwnerSee(true);
	FirstPersonArmsMeshComponent->SetOnlyOwnerSee(true);

	//First Person Shadows
	FirstPersonFullBodyMeshComponent->SetCastHiddenShadow(true);
	FirstPersonFullBodyMeshComponent->SetCastShadow(true);
	FirstPersonLegsMeshComponent->SetCastShadow(false);
	FirstPersonArmsMeshComponent->SetCastShadow(false);

	//First Person Master Pose Component
	//FirstPersonLegsMeshComponent->SetMasterPoseComponent(FirstPersonFullBodyMeshComponent,false);
	//FirstPersonArmsMeshComponent->SetMasterPoseComponent(FirstPersonFullBodyMeshComponent,false);

	//Control Rotation
	AsPawn->bUseControllerRotationRoll = false;
	AsPawn->bUseControllerRotationPitch = false;
	AsPawn->bUseControllerRotationYaw = !bIsThirdPerson;
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Math

float UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleDelta(const float StartAngle, const float EndAngle)
{
	float Delta = EndAngle - StartAngle;
	if (Delta > 180.0)   Delta -= 360.0f;
	else if (Delta < -180.0f) Delta += 360.0f;
	return Delta;
}

float UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleWrap(float Angle)
{
	Angle = FMath::Fmod(Angle + 180.0f, 360.0f);
	if (Angle < 0.0f) Angle += 360.0f;
	Angle -= 180.0f;
	return Angle;
}

float UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetFloatMoveToward(const float Start, const float End, const float MaxDelta)
{
	if (FMath::Abs(End - Start) <= MaxDelta)
		return End;
	else
		return Start + FMath::Sign(End - Start) * MaxDelta;
}

FVector UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetBoneWorldLocation(const FTransform& InBoneTransform_CS, FAnimInstanceProxy* InAnimInstanceProxy)
{
	return InAnimInstanceProxy->GetComponentTransform().TransformPosition(InBoneTransform_CS.GetLocation());
}

FVector UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetBoneWorldLocation(const FVector& InBoneLocation_CS, FAnimInstanceProxy* InAnimInstanceProxy)
{
	return InAnimInstanceProxy->GetComponentTransform().TransformPosition(InBoneLocation_CS);
}
