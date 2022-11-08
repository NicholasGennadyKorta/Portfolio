// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "FluidNarratorRuntime.h"
#include "Engine/DeveloperSettings.h"
#include "FluidNarratorDeveloperSettings.generated.h"

/**
 * 
 */

class UFluidNarratorLocalization;

//Fluid Narrator Developer Settings
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Fluid Narrator"))
class FLUIDNARRATORRUNTIME_API UFluidNarratorDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UFluidNarratorDeveloperSettings();
	
	//Text Block Style Colors Settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Normal", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorNormal;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Comment", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorComment;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Definition", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorDefinition;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Namespace", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorNamespace;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Language", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorLanguage;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Variables", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorVariables;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Section", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorSection;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Selector", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorSelector;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Event", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorEvent;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Operator", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorOperator;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName = "Responses", Category = "Screenplay Syntax Highlighting (Editor)")
		FColor TextBlockStyleColorResponse;

	//Other Settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Screenplay Other (Editor)")
		FColor BackgroundColorTint;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Screenplay Other (Editor)")
		FColor BackgroundReadOnlyColorTint;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Screenplay Other (Editor)")
		float FontSize;
	
	//Translate
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Translate", DisplayName = "Project Number/ID", meta=(EditCondition="bTranslateShowDetails", EditConditionHides))
		FString TranslateProjectNumberOrID;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Translate", DisplayName = "Key",  meta=(EditCondition="bTranslateShowDetails", EditConditionHides))
		FString TranslateAPIKey;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Translate", DisplayName = "Provider", meta=(EditCondition="bTranslateShowDetails", EditConditionHides))
		EFluidNarratorTranslatorProvider TranslatorProvider;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Translate", DisplayName = "Show Details")
		bool bTranslateShowDetails;
};