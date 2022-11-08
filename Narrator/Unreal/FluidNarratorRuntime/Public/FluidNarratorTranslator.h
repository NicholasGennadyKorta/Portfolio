// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidNarratorRuntime.h"
#include "CoreMinimal.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "HttpModule.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonSerializer.h"
#include "FluidNarratorTranslator.generated.h"

/**
* 
*/

UENUM(Blueprinttype)
enum class EFluidNarratorTranslatorProvider : uint8
{
	GoogleCloudV2,
	GoogleCloudV3
};

USTRUCT(BlueprintType)
struct FLUIDNARRATORRUNTIME_API FFluidNarratorTranslatorResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		TArray<FString> SourceTexts;
	UPROPERTY(BlueprintReadOnly)
		TArray<FString> TranslatedTexts;
	UPROPERTY(BlueprintReadOnly)
		FString TranslatedTextsRawJson;
	UPROPERTY(BlueprintReadOnly)
		FString Error;
	UPROPERTY(BlueprintReadOnly)
		bool bSuccessful;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFluidNarratorTranslatorResult, const  FFluidNarratorTranslatorResult&, Result_Out);

UCLASS(BlueprintType)
class FLUIDNARRATORRUNTIME_API UFluidNarratorTranslator : public UObject
{
	GENERATED_BODY()

public:
	UFluidNarratorTranslator(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable,DisplayName = "Set API Key (Google Cloud Translate)")
		void SetAPIKey(const FString APIKeyIn);
	UFUNCTION(BlueprintCallable,DisplayName = "Set Project Number Or ID (Google Cloud Translate)")
		void SetProjectNumberOrID(const FString ProjectNumberOrIDIn);
	UFUNCTION(BlueprintCallable)
		void SetTranslatorProvider(const EFluidNarratorTranslatorProvider TranslatorProviderIn);
	UFUNCTION(BlueprintCallable)
		void Translate(const FString SourceTextIn, const FString SourceLanguageCodeIn, const FString TargetLanguageCodeIn);
	UFUNCTION(BlueprintCallable)
		void TranslateMulti(const TArray<FString> SourceTextsIn, const FString SourceLanguageCodeIn, const FString TargetLanguageCodeIn);
	void ParseHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful);
	void ParseSourceTextToJsonString(FString& String_Ou);
	void ParseTranslatedTextFromJsonString(const FString JsonString, TArray<FString>& TranslatedTexts_Out) const;
	
	UPROPERTY(BlueprintAssignable, Category = "GoogleTranslate")
		FOnFluidNarratorTranslatorResult OnFluidNarratorTranslatorResult;
	UPROPERTY(BlueprintReadOnly)
		FFluidNarratorTranslatorResult TranslatorResult;
	
	TArray<FString> SourceInputs;
	FString ProjectNumberOrID;
	FString APIKey;
	FString SourceLanguageCode;
	FString TargetLanguageCode;
	FString Model;
	EFluidNarratorTranslatorProvider TranslatorProvider;
	bool bIsComplete;

private:
	FHttpModule* Http;
	FString PostEndpoint;
};
