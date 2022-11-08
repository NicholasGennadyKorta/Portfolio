// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidEntitiesCharacterHybridAnim.h"
#include "GameFramework/Character.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Entities Character Hybrid Anim Library Initializer

UFluidEntitiesCharacterHybridAnimLibrary::UFluidEntitiesCharacterHybridAnimLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Fluid Entities Character Hybrid Anim Library Root Motion Extractor

void UFluidEntitiesCharacterHybridAnimLibrary::ExtractRootMotion(UAnimSequence* AnimSequence, const bool bDeleteAllExistingCurves)
{
	//Return If No Animation Sequence
	if (AnimSequence == nullptr)
		return;
    
    //Remove All Curves
    //if (bDeleteAllExistingCurves)
   //     UAnimationBlueprintLibrary::RemoveAllCurveData(AnimSequence);
    
    //Extract Root Motion
    //ExtractRootMotionVelocity(AnimSequence);
   // ExtractRootMotionDirection(AnimSequence);
   // ExtractRootMotionDistance(AnimSequence);
	//ExtractRootMotionRotation(AnimSequence);
}

void UFluidEntitiesCharacterHybridAnimLibrary::ExtractRootMotionVelocity(UAnimSequence* AnimSequence)
{
	/*//Return If No Animation Sequence
	if (AnimSequence == nullptr)
		return;
    
    //Animation Sequence Properties
    int NumFrames = 0; UAnimationBlueprintLibrary::GetNumFrames(AnimSequence,NumFrames);
    float SequenceLength = 0; UAnimationBlueprintLibrary::GetSequenceLength(AnimSequence,SequenceLength);
    float Delta = SequenceLength / NumFrames;
    float TimeAtFrame = 0;
    
	//Remove Old Velocity Curve
    if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"Velocity",ERawCurveTrackTypes::RCT_Float))
        UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"Velocity",false);
    
    //Add Velocity Curve
	UAnimationBlueprintLibrary::AddCurve(AnimSequence,"Velocity");
    
    //Pose Variables
    FTransform Pose;
    FVector PosePreviousLocation;
    float PoseVelocity = 0;
	
    //First Frame Pose Location
    UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence,"Root",0,false,Pose);
    UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence,0,TimeAtFrame);
    PosePreviousLocation = Pose.GetLocation();
    
	for (int i = 1; i < NumFrames; i++)
	{
		//Get Pose | Time At Current Frame
		UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence,"Root",i,false,Pose);
		UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence,i,TimeAtFrame);

		//Get Velocity | Set Previous Location
		PoseVelocity  = (((Pose.GetLocation() -PosePreviousLocation).Size()) / Delta);
        PosePreviousLocation = Pose.GetLocation();
		
		//Write To Curve
		UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"Velocity",TimeAtFrame,PoseVelocity);
	}*/
}

void UFluidEntitiesCharacterHybridAnimLibrary::ExtractRootMotionDirection(UAnimSequence* AnimSequence)
{
    /*//Return If No Animation Sequence
    if (AnimSequence == nullptr)
        return;
    
    //Animation Sequence Properties
    int NumFrames = 0; UAnimationBlueprintLibrary::GetNumFrames(AnimSequence,NumFrames);
    float SequenceLength = 0; UAnimationBlueprintLibrary::GetSequenceLength(AnimSequence,SequenceLength);
    float Delta = SequenceLength / NumFrames;
    float TimeAtFrame = 0;
    
    //Remove Old Direction Curves
    if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"DirectionX",ERawCurveTrackTypes::RCT_Float))
        UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"DirectionX",false);
    if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"DirectionY",ERawCurveTrackTypes::RCT_Float))
        UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"DirectionY",false);
    if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"DirectionZ",ERawCurveTrackTypes::RCT_Float))
        UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"DirectionZ",false);

    //Add Direction Curves
    UAnimationBlueprintLibrary::AddCurve(AnimSequence,"DirectionX");
    UAnimationBlueprintLibrary::AddCurve(AnimSequence,"DirectionY");
    UAnimationBlueprintLibrary::AddCurve(AnimSequence,"DirectionZ");
    
    //Pose Variables
    FTransform Pose;
    FVector PosePreviousLocation;
    FVector PoseDirection;
    
    //First Frame Pose Location
    UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence,"Root",0,false,Pose);
    UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence,0,TimeAtFrame);
    PosePreviousLocation = Pose.GetLocation();
    
    for (int i = 1; i < NumFrames; i++)
    {
        //Get Pose | Time At Current Frame
        UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence,"Root",i,false,Pose);
        UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence,i,TimeAtFrame);

        //Get Direction | Set Previous Location
        PoseDirection = (Pose.GetLocation() - PosePreviousLocation).GetSafeNormal();
        PosePreviousLocation = Pose.GetLocation();

        //Write To Curve
        UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"DirectionX",TimeAtFrame,PoseDirection.X);
        UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"DirectionY",TimeAtFrame,PoseDirection.Y);
        UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"DirectionZ",TimeAtFrame,PoseDirection.Z);
    }*/
}

void UFluidEntitiesCharacterHybridAnimLibrary::ExtractRootMotionDistance(UAnimSequence* AnimSequence)
{
    // //Return If No Animation Sequence
    // if (AnimSequence == nullptr)
    //     return;
    //
    // //Animation Sequence Properties
    // int NumFrames = 0; UAnimationBlueprintLibrary::GetNumFrames(AnimSequence,NumFrames);
    // float SequenceLength = 0; UAnimationBlueprintLibrary::GetSequenceLength(AnimSequence,SequenceLength);
    // float Delta = SequenceLength / NumFrames;
    // float TimeAtFrame = 0;
    //
    // //Remove Old Distance Curve
    // if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"Distance",ERawCurveTrackTypes::RCT_Float))
    //     UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"Distance",false);
    // if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"DistanceReversed",ERawCurveTrackTypes::RCT_Float))
    //     UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"DistanceReversed",false);
    //
    // //Add Distance Curve
    // UAnimationBlueprintLibrary::AddCurve(AnimSequence,"Distance");
    // UAnimationBlueprintLibrary::AddCurve(AnimSequence,"DistanceReversed");
    //
    // //Pose Variables
    // FTransform Pose;
    // FVector PoseStartLocation;
    // float PoseDistance = 0;
    // //First Frame Pose Location
    //
    // UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence,"Root",0,false,Pose);
    // UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence,0,TimeAtFrame);
    // PoseStartLocation = Pose.GetLocation();
    //
    // // Distance Curve
    // for (int i = 1; i < NumFrames; i++)
    // {
    //     //Get Pose | Time At Current Frame
    //     UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence,"Root",i,false,Pose);
    //     UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence,i,TimeAtFrame);
    //
    //     //Get Distance Stop
    //     PoseDistance = FMath::Abs((Pose.GetLocation() - PoseStartLocation).Size());
    //
    //     //Write To Curve
    //     UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"Distance",TimeAtFrame,PoseDistance);
    // }
    //
    // //Distance Reverse Curve
    // {
    //     //Copy Distance Curve
    //     TArray< float > Times;
    //     TArray< float > Values;
    //     UAnimationBlueprintLibrary::GetFloatKeys(AnimSequence,"Distance",Times,Values);
    //     
    //     //Reverse Values
    //     int j = Times.Num() - 1;
    //     for (int i = 0; i < Times.Num(); i++)
    //     {
    //         UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"DistanceReversed",Times[j],Values[i]);
    //         j--;
    //     }
    // }
}

void UFluidEntitiesCharacterHybridAnimLibrary::ExtractRootMotionRotation(UAnimSequence* AnimSequence)
{
    // //Return If No Animation Sequence
    // if (AnimSequence == nullptr)
    //     return;
    //
    // //Animation Sequence Properties
    // int NumFrames = 0; UAnimationBlueprintLibrary::GetNumFrames(AnimSequence,NumFrames);
    // float SequenceLength = 0; UAnimationBlueprintLibrary::GetSequenceLength(AnimSequence,SequenceLength);
    // float Delta = SequenceLength / NumFrames;
    // float TimeAtFrame = 0;
    //
    // //Remove Old Rotation Curves
    // if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"RotationRoll",ERawCurveTrackTypes::RCT_Float))
    //     UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"RotationRoll",false);
    // if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"RotationPitch",ERawCurveTrackTypes::RCT_Float))
    //     UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"RotationPitch",false);
    // if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence,"RotationYaw",ERawCurveTrackTypes::RCT_Float))
    //     UAnimationBlueprintLibrary::RemoveCurve(AnimSequence,"RotationYaw",false);
    //
    // //Create Rotaiton Curves
    // UAnimationBlueprintLibrary::AddCurve(AnimSequence,"RotationRoll");
    // UAnimationBlueprintLibrary::AddCurve(AnimSequence,"RotationPitch");
    // UAnimationBlueprintLibrary::AddCurve(AnimSequence,"RotationYaw");
    //
    // //Loop Through All Frames
    // FTransform Pose;
    // float PoseRoll = 0;
    // float PosePitch = 0;
    // float PoseYaw = 0;
    //
    // for (int i = 1; i < AnimSequence->GetNumberOfSampledKeys(); i++)
    // {
    //     //Get Pose | Time At Current Frame
    //     UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence,"Root",i,true,Pose);
    //     UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence,i,TimeAtFrame);
    //
    //     //Get Roll Pitch Yaw
    //     UKismetMathLibrary::BreakRotator(Pose.Rotator(),PoseRoll,PosePitch,PoseYaw);
    //
    //     //Write To Curve
    //     UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"RotationRoll",TimeAtFrame,PoseRoll);
    //     UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"RotationPitch",TimeAtFrame,PosePitch);
    //     UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence,"RotationYaw",TimeAtFrame,PoseYaw);
    // }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Entities Character Hybrid Anim Instance : Entities

void UFluidEntitiesCharacterHybridAnimInstance::ReadEntityData(const int Entity)
{
	//Don't Do If Out Of Bounds
	if (Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Entities Character Hybrid Anim Instance : Action

void UFluidEntitiesCharacterHybridAnimInstance::DoAction(UAnimMontage* AnimMontage, bool bForceOverrideCurrentMontage, bool bMustBeGrounded, bool& bPlayedMontage_out)
{
	//Return If Not Valid Conditions
	if (AnimMontage == nullptr || Character == nullptr || bMustBeGrounded && Character->GetMovementComponent()->IsFalling())
		return;

	//Start Montage
	bPlayedMontage_out = false;
	if (!IsAnyMontagePlaying() || bForceOverrideCurrentMontage && AnimMontage != Character->GetCurrentMontage() || bCanOverrideAction && AnimMontage != Character->GetCurrentMontage())
	{
		ActionSectionIndex = 0;
		bLockActionInput = 0;
		bCanOverrideAction = false;
		bPlayedMontage_out = true;
	}

	//Set To Do Next Montage Section
	else if (bCanDoNextActionSection)
	{
		bCanDoNextActionSection = false;
		bDoNextActionSection = true;
	}
    
    //Actually Do Montage
    if (bPlayedMontage_out)
        DoPlayMontage(Character->GetMesh(),AnimMontage,1,0,"0");
}

void UFluidEntitiesCharacterHybridAnimInstance::DoActionNotifies(FName NotifyName)
{
    bool bReturnVarToForceAsFunctionOverEvent;
	DoActionNextNotify(NotifyName);
	DoActionEndNotify(NotifyName);
	DoActionInputLockNotify(NotifyName);
	DoActionCanOverrideNotify(NotifyName);
	//Character->OoActionEvent(NotifyName);
}

void UFluidEntitiesCharacterHybridAnimInstance::DoActionNextNotify(FName NotifyName)
{
	//Next Window
	if (NotifyName == "NextWindow")
		bCanDoNextActionSection = true;

	//Next Window Loop
	if (NotifyName == "NextWindowLoop")
	{
		bCanDoNextActionSection = true;
		ActionSectionIndex = -1;
	}
}

void UFluidEntitiesCharacterHybridAnimInstance::DoActionEndNotify(FName NotifyName)
{
	//Return If Invalid Character | Not End
	if (Character == nullptr || NotifyName != "End")
		return;

	//Do Next Montage Section
	if (bDoNextActionSection)
	{
		ActionSectionIndex++;
		Montage_JumpToSection(FName(FString::FromInt(ActionSectionIndex)));
		bCanDoNextActionSection = false;
		bDoNextActionSection = false;
		bLockActionInput = false;
		bCanOverrideAction = false;
	}

	//Do Actual End
	else
	{
		Character->StopAnimMontage();
		bLockActionInput = false;
		bCanDoNextActionSection = false;
		bDoNextActionSection = false;
		bCanOverrideAction = false;
	}
}

void UFluidEntitiesCharacterHybridAnimInstance::DoActionCanOverrideNotify(FName NotifyName)
{
	//Override True
	if (NotifyName == "CanOverrideTrue")
		bCanOverrideAction = true;

	//Override False
	if (NotifyName == "CanOverrideFalse")
		bCanOverrideAction = false;
}


void UFluidEntitiesCharacterHybridAnimInstance::DoActionInputLockNotify(FName NotifyName)
{
	//Lock Montage Input
	if (NotifyName == "InputLock")
		bLockActionInput = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Entities Character Hybrid Anim Instance Anim Montage

void UFluidEntitiesCharacterHybridAnimInstance::DoPlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent,class UAnimMontage* MontageToPlay,float MontagePlayRate,float StartingPosition,FName StartingSection)
{
    bool bPlayedSuccessfully = false;
    if (InSkeletalMeshComponent)
    {
        if (UAnimInstance* AnimInstance = InSkeletalMeshComponent->GetAnimInstance())
        {
            const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, MontagePlayRate, EMontagePlayReturnType::MontageLength, StartingPosition);
            bPlayedSuccessfully = (MontageLength > 0.f);

            if (bPlayedSuccessfully)
            {
                AnimInstancePtr = AnimInstance;
                if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
                {
                    MontageInstanceID = MontageInstance->GetInstanceID();
                }

                if (StartingSection != NAME_None)
                {
                    AnimInstance->Montage_JumpToSection(StartingSection, MontageToPlay);
                }
                
                BlendingOutDelegate.BindUObject(this, &UFluidEntitiesCharacterHybridAnimInstance::OnMontageBlendingOut);
                AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

                MontageEndedDelegate.BindUObject(this, &UFluidEntitiesCharacterHybridAnimInstance::OnMontageEnded);
                AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

                AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UFluidEntitiesCharacterHybridAnimInstance::OnNotifyBeginReceived);
                AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UFluidEntitiesCharacterHybridAnimInstance::OnNotifyEndReceived);
            }
        }
    }

    if (!bPlayedSuccessfully)
    {
    }
}

void UFluidEntitiesCharacterHybridAnimInstance::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
    if (IsNotifyValid(NotifyName, BranchingPointNotifyPayload))
    {
        DoActionNotifies(NotifyName);
    }
}


void UFluidEntitiesCharacterHybridAnimInstance::OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
    if (IsNotifyValid(NotifyName, BranchingPointNotifyPayload))
    {
    }
}


void UFluidEntitiesCharacterHybridAnimInstance::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
    {
        //OnInterrupted.Broadcast(NAME_None);
        bInterruptedCalledBeforeBlendingOut = true;
    }
    else
    {
        //OnBlendOut.Broadcast(NAME_None);
    }
}

void UFluidEntitiesCharacterHybridAnimInstance::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!bInterrupted)
    {
        //OnCompleted.Broadcast(NAME_None);
    }
    else if (!bInterruptedCalledBeforeBlendingOut)
    {
        //OnInterrupted.Broadcast(NAME_None);
    }

    UnbindDelegates();
}

bool UFluidEntitiesCharacterHybridAnimInstance::IsNotifyValid(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) const
{
    return ((MontageInstanceID != INDEX_NONE) && (BranchingPointNotifyPayload.MontageInstanceID == MontageInstanceID));
}

void UFluidEntitiesCharacterHybridAnimInstance::UnbindDelegates()
{
    if (UAnimInstance* AnimInstance = AnimInstancePtr.Get())
    {
        AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UFluidEntitiesCharacterHybridAnimInstance::OnNotifyBeginReceived);
        AnimInstance->OnPlayMontageNotifyEnd.RemoveDynamic(this, &UFluidEntitiesCharacterHybridAnimInstance::OnNotifyEndReceived);
    }
}
