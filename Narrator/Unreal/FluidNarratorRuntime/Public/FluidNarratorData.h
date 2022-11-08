// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "UObject/Object.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"
#include "FluidNarratorData.generated.h"

/**
 * 
 */

//Fluid Narrator Localization Culture
USTRUCT(BlueprintType)
struct FLUIDNARRATORRUNTIME_API FFluidNarratorLocalizationCulture
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Language;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName NativeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName LanguageCode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName TranslatorAgency;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Translator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Version;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Date;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsNativeCulture;
};

//Fluid Narrator Localization Text Import Json Export
USTRUCT()
struct FLUIDNARRATORRUNTIME_API FFluidNarratorLocalizationTextImportExportJsonElement
{
	GENERATED_BODY()

	UPROPERTY()
		FString TranslatorComment;
	UPROPERTY()
		FString Namespace;
	UPROPERTY()
		FString Source;
	UPROPERTY()
		FString Translation;
};

//Fluid Narrator Localization Text Import Export Json
USTRUCT()
struct FLUIDNARRATORRUNTIME_API FFluidNarratorLocalizationImportExportJson
{
	GENERATED_BODY()

	UPROPERTY()
		FString Copyright;
	UPROPERTY()
		FString License;
	UPROPERTY()
		FString Language;
	UPROPERTY()
		FString NativeName;
	UPROPERTY()
		FString LanguageCode;
	UPROPERTY()
		FString PreviousTranslatorAgency;
	UPROPERTY()
		FString NewTranslatorAgency;
	UPROPERTY()
		FString PreviousTranslator;
	UPROPERTY()
		FString NewTranslator;
	UPROPERTY()
		FString PreviousVersion;
	UPROPERTY()
		FString NewVersion;
	UPROPERTY()
		FString PreviousDate;
	UPROPERTY()
		FString NewDate;
	UPROPERTY()
		TArray<FFluidNarratorLocalizationTextImportExportJsonElement> Texts;
};

//Fluid Narrator Localization Text Pair
USTRUCT()
struct FLUIDNARRATORRUNTIME_API FFluidNarratorLocalizationText
{
	GENERATED_BODY()
	
	UPROPERTY()
		FName Translation;
	UPROPERTY()
		bool bIsAutoTranslated;
};

//Fluid Narrator Localization Text Namespace Culture Pair
USTRUCT()
struct FLUIDNARRATORRUNTIME_API FFluidNarratorLocalizationTextPair
{
	GENERATED_BODY()

	UPROPERTY()
		FName Namespace;
	UPROPERTY()
		FName Culture;
	UPROPERTY()
		FName Source;

	bool operator == (const  FFluidNarratorLocalizationTextPair& Pair) const
	{
		return Namespace == Pair.Namespace && Culture == Pair.Culture && Source == Pair.Source;
	}
};

//Fluid Narrator Localization Text Namespace Culture Pair Hash
FORCEINLINE uint32 GetTypeHash(const FFluidNarratorLocalizationTextPair& Pair)
{
	return FCrc::MemCrc32(&Pair,sizeof(FFluidNarratorLocalizationTextPair));
}

//Fluid Narrator Localization
UCLASS(BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorLocalization : public UObject
{
	GENERATED_BODY()

// Import Export
public:

	void ExportImportCreateDirectories();
	void FilesExportJson();
	void FilesImportJson();
	void FilesExportTsv();
	void FilesImportTsv();

//Get Set Localization Texts	
public:

	void SetLocalizationTexts(const TArray<FName> NamespacesIn, const TArray<FName> CulturesIn, const TArray<FString> SourcesIn, const TArray<FString> TranslationsIn, const TArray<bool> bAutoTranslatedsIn);
	void GetLocalizationTexts(TArray<FName>& Namespaces_Out, TArray<FName>& Cultures_Out, TArray<FString>& Sources_Out, TArray<FString>& Translations_Out, TArray<bool>& bAutoTranslateds_Out);
	void GetLocalizationTextAsString(const FName CultureIn, const FString StringIn, FString& String_Out);
	void GetLocalizationTextAsText(const FName CultureIn, const FText TextIn, FText& Text_Out);
	void GetLocalizationTextAsTexts(const FName CultureIn, const TArray<FText> TextsIn, TArray<FText>& Texts_Out);
	void GetLocalizationTextAsName(const FName CultureIn, const FName NameIn, FName& Name_Out);
	void GetLocalizationTextAsNames(const FName CultureIn, const TArray<FName> NamesIn, TArray<FName>& Names_Out);

//Properties	
public:

	UPROPERTY(EditDefaultsOnly)
		FName Copyright;
	UPROPERTY(EditDefaultsOnly)
		FName License;
	UPROPERTY(EditDefaultsOnly)
		TArray<FName> Namespaces;
	UPROPERTY()
		TMap<FName,FFluidNarratorLocalizationCulture> Cultures;
	UPROPERTY()
		TMap<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText> Texts;
};

//Fluid Narrator Screenplay
UCLASS(BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorScreenplay : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString Text;
};