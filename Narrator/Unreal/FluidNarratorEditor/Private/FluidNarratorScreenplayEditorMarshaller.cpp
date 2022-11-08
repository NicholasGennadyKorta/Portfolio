// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidNarratorScreenplayEditorMarshaller.h"
#include "Framework/Text/IRun.h"
#include "Framework/Text/TextLayout.h"
#include "Framework/Text/ISlateRun.h"
#include "Framework/Text/SlateTextRun.h"
#include "Misc/ExpressionParserTypes.h"

/*=============================================================================
Fluid Narrator Screenplay Marshaller : Token Texts
=============================================================================*/

const TArray<FString> FluidNarratorEditorMarshallerDefinitionTokenTexts = { FluidNarratorScreenplaySystemSyntax::Definition };
const TArray<FString> FluidNarratorEditorMarshallerNamespaceTokenTexts = { FluidNarratorScreenplaySystemSyntax::Namespace };
const TArray<FString> FluidNarratorEditorMarshallerVariableTokenTexts =  { FluidNarratorScreenplaySystemSyntax::Variable, FluidNarratorScreenplaySystemSyntax::VariableCheck, FluidNarratorScreenplaySystemSyntax::VariableSet };
const TArray<FString> FluidNarratorEditorMarshallerCancelTokenTexts = {",","(",")","="};;
const TArray<FString> FluidNarratorEditorMarshallerSectionTokenTexts = { FluidNarratorScreenplaySystemSyntax::Section, FluidNarratorScreenplaySystemSyntax::SectionGoTo };
const TArray<FString> FluidNarratorEditorMarshallerSeparatorSelectorTokenTexts = {FluidNarratorScreenplaySystemSyntax::Separator, FluidNarratorScreenplaySystemSyntax::SelectorStart, FluidNarratorScreenplaySystemSyntax::SelectorEnd };
const TArray<FString> FluidNarratorEditorMarshallerEventTokenTexts = {FluidNarratorScreenplaySystemSyntax::Event};
const TArray<FString> FluidNarratorEditorMarshallerTextAndResponseTokenTexts = {FluidNarratorScreenplaySystemSyntax::Text, FluidNarratorScreenplaySystemSyntax::Response, FluidNarratorScreenplaySystemSyntax::ResponseDefault ,FluidNarratorScreenplaySystemSyntax::TextDefaultCharacterID, FluidNarratorScreenplaySystemSyntax::ResponseDefaultCharacterID };

/*=============================================================================
Fluid Narrator Screenplay Marshaller : Static
=============================================================================*/

TSharedRef<FluidNarratorScreenplayMarshaller> FluidNarratorScreenplayMarshaller::Create()
{
	TArray<FSyntaxTokenizer::FRule> TokenizerRules;
	
	//Load Token Texts
	for (const auto& Item : FluidNarratorEditorMarshallerDefinitionTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	for (const auto& Item : FluidNarratorEditorMarshallerNamespaceTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	for (const auto& Item : FluidNarratorEditorMarshallerVariableTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	for (const auto& Item : FluidNarratorEditorMarshallerCancelTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	for (const auto& Item : FluidNarratorEditorMarshallerSectionTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	for (const auto& Item : FluidNarratorEditorMarshallerSeparatorSelectorTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	for (const auto& Item : FluidNarratorEditorMarshallerEventTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	for (const auto& Item :FluidNarratorEditorMarshallerTextAndResponseTokenTexts)
		TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Item));
	
	return MakeShareable(new FluidNarratorScreenplayMarshaller(FSyntaxTokenizer::Create(TokenizerRules)));
}

/*=============================================================================
Fluid Narrator Screenplay Marshaller : Object
=============================================================================*/

FluidNarratorScreenplayMarshaller::FluidNarratorScreenplayMarshaller(TSharedPtr< FSyntaxTokenizer > InTokenizer) : FSyntaxHighlighterTextLayoutMarshaller(MoveTemp(InTokenizer))
{
}

void FluidNarratorScreenplayMarshaller::UpdateTokensStyle()
{
	//Get Developer Settings
	const UFluidNarratorDeveloperSettings* DeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
	
	//Font 
	const auto Font = FCoreStyle::GetDefaultFontStyle("mono",DeveloperSettings->FontSize);
	
	//Normal
	TextBlockStyleNormal = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorNormal)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));

	//Comment
	TextBlockStyleComment = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorComment)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));

	//Definition
	TextBlockStyleDefinition = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorDefinition)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));

	//Namespace
	TextBlockStyleNamespace = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorNamespace)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));
	
	//Variables
	TextBlockStyleVariables = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorVariables)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));
	
	//Section
	TextBlockStyleSection  = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorSection)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));;

	//Selector
	TextBlockStyleSelector =  FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorSelector)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));

	//Event 
	TextBlockStyleEvent = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorEvent)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));;

	//Operator
	TextBlockStyleOperator  = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorOperator)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));

	//Responses
	TextBlockStyleResponses = FTextBlockStyle()
	.SetFont(Font)
	.SetColorAndOpacity(DeveloperSettings->TextBlockStyleColorResponse)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetHighlightColor(FLinearColor(0.02f, 0.3f, 0.0f));
}

void FluidNarratorScreenplayMarshaller::ParseTokens(const FString& SourceString, FTextLayout& TargetTextLayout, TArray<FSyntaxTokenizer::FTokenizedLine> TokenizedLines)
{
	UpdateTokensStyle();
	
	//Parse State (Declare Here For Cleaner Code)
	enum class ETokenParseState : uint8
	{
		None,
		Comment,
		Definition,
		Namespace,
		Variable,
		Selector,
		SelectorEnd,
		Section,
		Response,
		Event,
	};
	
	// Parse the tokens, generating the styled runs for each line
	ETokenParseState TokenParseState = ETokenParseState::None;
	TArray<FTextLayout::FNewLineData> NewLineDatas;
	
	for (int32 i = 0; i < TokenizedLines.Num(); i++)
	{
		TokenParseState = ETokenParseState::None;
		TSharedRef<FString> ModelString = MakeShareable(new FString());
		TArray<TSharedRef<IRun>> Runs;
		
		for (int32 x = 0; x < TokenizedLines[i].Tokens.Num(); x++)
		{
			const FString TokenText = SourceString.Mid(TokenizedLines[i].Tokens[x].Range.BeginIndex, TokenizedLines[i].Tokens[x].Range.Len());
			const FTextRange ModelRange(ModelString->Len(), ModelString->Len() + TokenText.Len());
			ModelString->Append(TokenText);
		
			FRunInfo RunInfo(TEXT("Normal"));
			FTextBlockStyle TextBlockStyle = TextBlockStyleNormal;

			//Not White Space
			if (!FString(TokenText).TrimEnd().IsEmpty())
			{
				//Comment (Single Line)
				if (TokenText == TEXT("/") || TokenParseState == ETokenParseState::Comment)
					TokenParseState = ETokenParseState::Comment;

				//Defenition
				else if (FluidNarratorEditorMarshallerDefinitionTokenTexts.Contains(TokenText) || TokenParseState == ETokenParseState::Definition)
					TokenParseState = ETokenParseState::Definition;
				else if (FluidNarratorEditorMarshallerNamespaceTokenTexts.Contains(TokenText) || TokenParseState == ETokenParseState::Namespace)
					TokenParseState = ETokenParseState::Namespace;
				//Other
				else
				{
					if (FluidNarratorEditorMarshallerSeparatorSelectorTokenTexts.Contains(TokenText.ToLower()))
						TokenParseState = ETokenParseState::Selector;
					if (FluidNarratorEditorMarshallerEventTokenTexts.Contains(TokenText) || TokenParseState == ETokenParseState::Event)
						TokenParseState = ETokenParseState::Event;
					if (FluidNarratorEditorMarshallerTextAndResponseTokenTexts.Contains(TokenText))
						TokenParseState = ETokenParseState::Response;

					//Variable
					if (FluidNarratorEditorMarshallerVariableTokenTexts.Contains(TokenText) || TokenParseState == ETokenParseState::Variable)
						TokenParseState = ETokenParseState::Variable;
					
					//Cancel
					if (FluidNarratorEditorMarshallerCancelTokenTexts.Contains(TokenText) && TokenParseState != ETokenParseState::None && TokenParseState != ETokenParseState::Definition)
						TokenParseState = ETokenParseState::None;
					else if (TokenText.IsNumeric() && TokenParseState != ETokenParseState::None && TokenParseState != ETokenParseState::Definition)
						TokenParseState = ETokenParseState::None;

					if (FluidNarratorEditorMarshallerSectionTokenTexts.Contains(TokenText) || TokenParseState == ETokenParseState::Section)
						TokenParseState = ETokenParseState::Section;
				}

				switch (TokenParseState)
				{
				case ETokenParseState::Definition:
					RunInfo.Name = TEXT("Definition");
					TextBlockStyle = TextBlockStyleDefinition;
					break;
				case ETokenParseState::Namespace:
					RunInfo.Name = TEXT("Namespace");
					TextBlockStyle = TextBlockStyleNamespace;
					break;
				case ETokenParseState::Variable:
					RunInfo.Name = TEXT("Variable");
					TextBlockStyle = TextBlockStyleVariables;
					break;
				case ETokenParseState::Section:
					RunInfo.Name = TEXT("Section");
					TextBlockStyle = TextBlockStyleSection;
					break;
				case ETokenParseState::Event:
					RunInfo.Name = TEXT("Event");
					TextBlockStyle = TextBlockStyleEvent;
					break;
				case ETokenParseState::Comment:
					RunInfo.Name = TEXT("Comment");
					TextBlockStyle = TextBlockStyleComment;
					break;
				case ETokenParseState::Selector:
					RunInfo.Name = TEXT("SelectorSingle");
					TextBlockStyle = TextBlockStyleSelector;
					TokenParseState = ETokenParseState::None;
					break;
				case ETokenParseState::SelectorEnd:
					RunInfo.Name = TEXT("SelectorEnd");
					TextBlockStyle = TextBlockStyleSelector;
					TokenParseState = ETokenParseState::None;
					break;
				case ETokenParseState::Response:
					RunInfo.Name = TEXT("Response");
					TextBlockStyle = TextBlockStyleResponses;
					TokenParseState = ETokenParseState::None;
					break;
				default:
					break;
				}
				
				TSharedRef< ISlateRun > Run = FSlateTextRun::Create(RunInfo, ModelString, TextBlockStyle, ModelRange);
				Runs.Add(Run);
			}
			else
			{
				RunInfo.Name = TEXT("WhiteSpace");
				if (TokenParseState != ETokenParseState::Comment)
					TokenParseState = ETokenParseState::None;
				
				TSharedRef< ISlateRun > Run = FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun::Create(RunInfo, ModelString, TextBlockStyle, ModelRange, 4);
				Runs.Add(Run);
			}
			
		}

		NewLineDatas.Emplace(MoveTemp(ModelString), MoveTemp(Runs));
	}

	TargetTextLayout.AddLines(NewLineDatas);
}

/*=============================================================================
//Fluid Narrator Screenplay Marshaller Syntax Whitespace Text Run
=============================================================================*/

TSharedRef< FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun > FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun::Create( const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& Style, const FTextRange& InRange, int32 NumSpacesPerTab )
{
	return MakeShareable( new FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun( InRunInfo, InText, Style, InRange, NumSpacesPerTab ) );
}

FVector2D FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun::Measure( int32 StartIndex, int32 EndIndex, float Scale, const FRunTextContext& TextContext) const
{
	const FVector2D ShadowOffsetToApply((EndIndex == Range.EndIndex) ? FMath::Abs(Style.ShadowOffset.X * Scale) : 0.0f, FMath::Abs(Style.ShadowOffset.Y * Scale));

	if ( EndIndex - StartIndex == 0 )
	{
		return FVector2D( ShadowOffsetToApply.X * Scale, GetMaxHeight( Scale ) );
	}

	// count tabs
	int32 TabCount = 0;
	for(int32 Index = StartIndex; Index < EndIndex; Index++)
	{
		if((*Text)[Index] == TEXT('\t'))
		{
			TabCount++;
		}
	}

	const TSharedRef< FSlateFontMeasure > FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	FVector2D Size = FontMeasure->Measure( **Text, StartIndex, EndIndex, Style.Font, true, Scale ) + ShadowOffsetToApply;

	Size.X -= TabWidth * (float)TabCount * Scale;
	Size.X += SpaceWidth * (float)(TabCount * NumSpacesPerTab) * Scale;

	return Size;
}

FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun::FUFluidNarratorScreenplayMarshallerSyntaxWhitespaceTextRun( const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& InStyle, const FTextRange& InRange, int32 InNumSpacesPerTab ) : FSlateTextRun(InRunInfo, InText, InStyle, InRange) , NumSpacesPerTab(InNumSpacesPerTab)
{
	// measure tab width
	const TSharedRef< FSlateFontMeasure > FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TabWidth = FontMeasure->Measure( TEXT("\t"), 0, 1, Style.Font, true, 1.0f ).X;
	SpaceWidth = FontMeasure->Measure( TEXT(" "), 0, 1, Style.Font, true, 1.0f ).X;
}

