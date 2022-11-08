// Copyright Epic Games, Inc. All Rights Reserved.


#include "FluidNarratorTranslator.h"

 UFluidNarratorTranslator:: UFluidNarratorTranslator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	//Get Developer Settings
 	const UFluidNarratorDeveloperSettings* DeveloperSettings = GetMutableDefault<UFluidNarratorDeveloperSettings>();
 	
 	//Get Http
    Http = &FHttpModule::Get();

 	//Set Default API Key And Project Number Or ID (Developer Settings)
 	SetAPIKey(DeveloperSettings->TranslateAPIKey);
 	SetProjectNumberOrID(DeveloperSettings->TranslateProjectNumberOrID);
 	
 	//Set Default Translate Provider
 	SetTranslatorProvider(DeveloperSettings->TranslatorProvider);
}

void UFluidNarratorTranslator::SetAPIKey(const FString APIKeyIn)
{
 	APIKey = APIKeyIn;
}

void UFluidNarratorTranslator::SetProjectNumberOrID(const FString ProjectNumberOrIDIn)
{
	ProjectNumberOrID = ProjectNumberOrIDIn;
}

void UFluidNarratorTranslator::SetTranslatorProvider(const EFluidNarratorTranslatorProvider TranslatorProviderIn)
{
 	if (TranslatorProvider == EFluidNarratorTranslatorProvider::GoogleCloudV2)
 		PostEndpoint = "https://translation.googleapis.com/language/translate/v2";
 	if (TranslatorProvider == EFluidNarratorTranslatorProvider::GoogleCloudV3)
 		PostEndpoint = "https://translation.googleapis.com/v3/projects/" + ProjectNumberOrID + ":translateText";
}

void UFluidNarratorTranslator::Translate(FString SourceTextIn, FString SourceLanguageCodeIn, FString TargetLanguageCodeIn)
{
 	//Set Translate Parameters
 	SourceInputs = { SourceTextIn };
 	TranslateMulti(SourceInputs,SourceLanguageCode,TargetLanguageCode);
}

void UFluidNarratorTranslator::TranslateMulti(TArray<FString> SourceTextsIn, FString SourceLanguageCodeIn, FString TargetLanguageCodeIn)
{
 	//Set Translate Parameters
 	SourceInputs = SourceTextsIn;
 	SourceLanguageCode = SourceLanguageCodeIn;
 	TargetLanguageCode = TargetLanguageCodeIn;

 	//Set Not Complete
 	bIsComplete = false;
 	
 	//Get Language URL
 	const FString URL = PostEndpoint + "?key=" + APIKey;
 	
 	//Create Http Request
 	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = Http->CreateRequest();
 	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UFluidNarratorTranslator::ParseHttpResponse);

 	//Get Text Inputs As JSON String
 	FString TextInputsAsJSONString;
 	ParseSourceTextToJsonString(TextInputsAsJSONString);

 	//Setup Http Request
 	HttpRequest->SetURL(URL);
 	HttpRequest->SetVerb("POST");
 	HttpRequest->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
 	HttpRequest->SetHeader("Content-Type", TEXT("application/json"));
 	HttpRequest->SetHeader(TEXT("Accepts"), TEXT("application/json"));
 	HttpRequest->SetContentAsString(TextInputsAsJSONString);
 	HttpRequest->ProcessRequest();
 }

void UFluidNarratorTranslator::ParseHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
{
 	//Successful
    if (Response->GetResponseCode() == 200)
    {
    	ParseTranslatedTextFromJsonString(Response->GetContentAsString(),TranslatorResult.TranslatedTexts);
    	TranslatorResult.SourceTexts = SourceInputs;
    	TranslatorResult.TranslatedTextsRawJson = Response->GetContentAsString();
    	TranslatorResult.Error = "";
    	TranslatorResult.bSuccessful = true;
    }

 	//Not Successful
    else
    {
    	TranslatorResult.TranslatedTexts.Empty();
    	TranslatorResult.SourceTexts = SourceInputs;
    	TranslatorResult.TranslatedTextsRawJson = "";
    	TranslatorResult.Error = "Not Successful";
    	TranslatorResult.bSuccessful = false;
    }

 	//Broadcast Translator Result
 	OnFluidNarratorTranslatorResult.Broadcast(TranslatorResult);
 	bIsComplete = true;
}

void UFluidNarratorTranslator::ParseSourceTextToJsonString(FString& String_Out)
{
 	//Query
 	TArray<TSharedPtr<FJsonValue>> Query;
 	for (int i = 0; i < SourceInputs.Num(); i++)
 		Query.Add(MakeShareable(new FJsonValueString(SourceInputs[i])));

 	//Create Json Root Object
 	const TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
 	
 	//Set Json Fields (Google API V2)
 	if (TranslatorProvider == EFluidNarratorTranslatorProvider::GoogleCloudV2)
 	{
 		JsonObject->SetArrayField("q", Query);
 		JsonObject->SetStringField("target", TargetLanguageCode);
 	}

 	//Set Json Fields (Google API V3)
 	if (TranslatorProvider == EFluidNarratorTranslatorProvider::GoogleCloudV3)
    {
    	JsonObject->SetStringField("sourceLanguageCode", SourceLanguageCode);
    	JsonObject->SetStringField("targetLanguageCode", TargetLanguageCode);
    	JsonObject->SetArrayField("contents", Query);
    }
 	
 	//Serialize
 	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&String_Out);
 	FJsonSerializer::Serialize(JsonObject, Writer);
}

 void UFluidNarratorTranslator::ParseTranslatedTextFromJsonString(const FString JsonString, TArray<FString>& TranslatedTexts_Out) const
 {
 	//Create Json Object | Reader
 	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
 	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
    	
 	//Default Outs
 	TranslatedTexts_Out.Empty();
 	
 	//Return If Not Valid
 	if (!JsonObject.IsValid() || !FJsonSerializer::Deserialize(JsonReader, JsonObject))
 		return;
 	
 	//Get Data | Translation Fields
 	const TSharedPtr<FJsonObject> Data = JsonObject->GetObjectField("data");
 	const TArray<TSharedPtr<FJsonValue>> Translations = Data->GetArrayField("translations");

 	//Add Translation Fields To Translated Texts Out
 	for (int32 i = 0; i < Translations.Num(); i++)
 		TranslatedTexts_Out.Add(Translations[i]->AsObject()->GetStringField("translatedText"));
 }

