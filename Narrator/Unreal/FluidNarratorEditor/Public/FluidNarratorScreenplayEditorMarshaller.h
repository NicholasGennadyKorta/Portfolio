// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "FluidNarratorRuntime.h"
#include "FluidNarratorEditor.h"
#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Text/SlateTextRun.h"
#include "Framework/Text/SyntaxTokenizer.h"
#include "Framework/Text/SyntaxHighlighterTextLayoutMarshaller.h"

/**
 * 
 */

class UFluidNarratorDeveloperSettings;
class FTextLayout;

DECLARE_DELEGATE_OneParam(FOnLineNumberChanged, const int32&)
DECLARE_DELEGATE_ThreeParams(FOnLineModified, const FString& ,const int32&, const int32&);

class FluidNarratorScreenplayMarshaller : public FSyntaxHighlighterTextLayoutMarshaller
{
// Static	
public:
	
	static TSharedRef< FluidNarratorScreenplayMarshaller > Create();
	
//Object
protected:

	FluidNarratorScreenplayMarshaller(TSharedPtr< FSyntaxTokenizer > InTokenizer);
	void UpdateTokensStyle();
	virtual void ParseTokens(const FString& SourceString, FTextLayout& TargetTextLayout, TArray<FSyntaxTokenizer::FTokenizedLine> TokenizedLines) override;

	//Text Block Styles
	FTextBlockStyle TextBlockStyleNormal;
	FTextBlockStyle TextBlockStyleComment;
	FTextBlockStyle TextBlockStyleDefinition;
	FTextBlockStyle TextBlockStyleNamespace;
	FTextBlockStyle TextBlockStyleVariables;
	FTextBlockStyle TextBlockStyleSection;
	FTextBlockStyle TextBlockStyleSelector;
	FTextBlockStyle TextBlockStyleEvent;
	FTextBlockStyle TextBlockStyleOperator;
	FTextBlockStyle TextBlockStyleResponses;
};

//Fluid Narrator Screenplay Marshaller Syntax Whitespace Text Run
class FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun : public FSlateTextRun
{
public:
	static TSharedRef< FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun > Create( const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& Style, const FTextRange& InRange, int32 NumSpacesPerTab );
	virtual FVector2D Measure( int32 StartIndex, int32 EndIndex, float Scale, const FRunTextContext& TextContext) const override;
	FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun( const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& InStyle, const FTextRange& InRange, int32 InNumSpacesPerTab );
	
	int32 NumSpacesPerTab;
	float TabWidth;
	float SpaceWidth;
};
