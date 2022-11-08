// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidNarratorDeveloperSettings.h"

/*=============================================================================
Fluid Narrator Screenplay Marshaller Syntax Developer Settings
=============================================================================*/

UFluidNarratorDeveloperSettings::UFluidNarratorDeveloperSettings()
{
	//Text Block Style Color Settings
	TextBlockStyleColorNormal = FColor::White;
	TextBlockStyleColorComment = FColor::FromHex("99999990");
	TextBlockStyleColorDefinition= FColor::FromHex("FF544D");
	TextBlockStyleColorNamespace = FColor::FromHex("0027FFFF");
	TextBlockStyleColorVariables = FColor::Emerald;
	TextBlockStyleColorSection = FColor::FromHex("4C84B4");
	TextBlockStyleColorSelector = FColor::Magenta;
	TextBlockStyleColorEvent = FColor::FromHex("E9E97A");
	TextBlockStyleColorOperator = FColor::Orange;
	TextBlockStyleColorResponse = FColor::FromHex("0027FFFF");
	
	//Other Settings
	BackgroundColorTint = FColor::White;
	BackgroundReadOnlyColorTint = FColor::FromHex("202020");
	FontSize = 10;
}