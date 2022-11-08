#pragma once

#include "FluidEntitiesCharacterHybridRuntime.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FluidEntitiesCharacterHybridSubsystem.h"
#include "FluidEntitiesCharacterHybridAnim.generated.h"

//Fluid Entities Character Hybrid Anim Library
UCLASS()
class FLUIDENTITIESCHARACTERHYBRIDRUNTIME_API UFluidEntitiesCharacterHybridAnimLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

// Fluid Entities Character Hybrid Anim Library Extract Root Motion
public:
	
	UFUNCTION(BlueprintCallable)
		static void ExtractRootMotion(UAnimSequence* AnimSequence, const bool bDeleteAllExistingCurves);
	UFUNCTION(BlueprintCallable)
		static void ExtractRootMotionVelocity(UAnimSequence* AnimSequence);
    UFUNCTION(BlueprintCallable)
        static void ExtractRootMotionDirection(UAnimSequence* AnimSequence);
    UFUNCTION(BlueprintCallable)
        static void ExtractRootMotionDistance(UAnimSequence* AnimSequence);
	UFUNCTION(BlueprintCallable)
		static void ExtractRootMotionRotation(UAnimSequence* AnimSequence);
};

//Fluid Entities Character Hybrid Anim Instance
UCLASS()
class FLUIDENTITIESCHARACTERHYBRIDRUNTIME_API UFluidEntitiesCharacterHybridAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

//Fluid Entities Character Hybrid Anim Instance : Entities
public:

	UFUNCTION(BlueprintCallable)
		void ReadEntityData(const int Entity);
	UPROPERTY(BlueprintReadOnly)
		FFluidEntitiesCharacterHybridTransformComponent EntityTransformComponent;
	UPROPERTY(BlueprintReadOnly)
		FFluidEntitiesCharacterHybridInputXYComponent EntityInputXYComponent;
	UPROPERTY(BlueprintReadOnly)
		FFluidEntitiesCharacterHybridInputYawComponent EntityInputYawComponent;
	UPROPERTY(BlueprintReadOnly)
		FFluidEntitiesCharacterHybridInputStateComponent EntityInputStateComponent;
	UPROPERTY(BlueprintReadOnly)
		FFluidEntitiesCharacterHybridDistanceMatchingComponent EntityDistanceMatchingComponent;
	UPROPERTY(BlueprintReadOnly)
		FFluidEntitiesCharacterHybridOrientationWarpingComponent EntityOrientationWarpingComponent;
	UPROPERTY(BlueprintReadOnly)
		FFluidEntitiesCharacterHybridMovementComponent EntityMovementComponent;
	
//Fluid Entities Character Hybrid Anim Instance : Action
public:

	UFUNCTION(BlueprintCallable)
		void DoAction(UAnimMontage* AnimMontage, bool bForceOverrideCurrentMontage, bool bMustBeGrounded, bool& bPlayedMontage_out);
	UFUNCTION(BlueprintCallable)
		void DoActionNotifies(FName NotifyName);
	UFUNCTION(BlueprintCallable)
	    void DoActionNextNotify(FName NotifyName);
	UFUNCTION(BlueprintCallable)
		void DoActionEndNotify(FName NotifyName);
	UFUNCTION(BlueprintCallable)
	    void DoActionInputLockNotify(FName NotifyName);
	UFUNCTION(BlueprintCallable)
		void DoActionCanOverrideNotify(FName NotifyName);

	UPROPERTY(BlueprintReadWrite)
		ACharacter* Character;
	
	int ActionSectionIndex;
	bool bLockActionInput;
	bool bDoNextActionSection;
	bool bCanDoNextActionSection;
	bool bCanOverrideAction;

//Fluid Entities Character Hybrid Anim Instance Anim Montage
public:
    
    UFUNCTION(BlueprintCallable)
        void DoPlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent,class UAnimMontage* MontageToPlay,float MontagePlayRate,float StartingPosition,FName StartingSection);
    UFUNCTION()
        void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
    UFUNCTION()
        void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    UFUNCTION()
        void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
    UFUNCTION()
        void OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
    bool IsNotifyValid(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) const;
    void UnbindDelegates();
    
    TWeakObjectPtr<UAnimInstance> AnimInstancePtr;
    int32 MontageInstanceID;
    uint32 bInterruptedCalledBeforeBlendingOut : 1;
    FOnMontageBlendingOutStarted BlendingOutDelegate;
    FOnMontageEnded MontageEndedDelegate;
};