// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FluidEntitiesCharacterHybridAnimNodes.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "UnrealWidget.h"
#include "Kismet2/CompilerResultsLog.h"
#include "FluidEntitiesCharacterHybridAnimNodesEditor.generated.h"

//Fluid Entities Character Hybrid Editor Forward Declarations
struct FFluidEntitiesCharacterHybridAnimNodeStrideWarping;

//Fluid Entities Character Hybrid Editor AnimGraphNode StrideWarping
UCLASS()
class FLUIDENTITIESCHARACTERHYBRIDEDITOR_API UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Settings")
		FFluidEntitiesCharacterHybridAnimNodeStrideWarping Node;
	
	UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping();
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void CopyPinDefaultsToNodeData(UEdGraphPin* InPin) override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const { return &Node; }

};
