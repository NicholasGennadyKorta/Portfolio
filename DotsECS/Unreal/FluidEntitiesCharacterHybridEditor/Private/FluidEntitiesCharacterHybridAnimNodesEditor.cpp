// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidEntitiesCharacterHybridAnimNodesEditor.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Editor AnimGraphNode StrideWarping

#define LOCTEXT_NAMESPACE "A3Nodes"

UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping::UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping()
{
}

FText UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping::GetTooltipText() const
{
	return LOCTEXT("Stride Warping Fluid Entities Character Hybrid", "Stride Warping Fluid Entities Character Hybrid");
}

FLinearColor UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping::GetNodeTitleColor() const
{
	return FLinearColor(0,0,0);
}

FText UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("Stride Warping Fluid Entities Character Hybrid", "Stride Warping Fluid Entities Character Hybrid");
}

void UFluidEntitiesCharacterHybridAnimGraphNodeStrideWarping::CopyPinDefaultsToNodeData(UEdGraphPin * InPin)
{
}

#undef LOCTEXT_NAMESPACE