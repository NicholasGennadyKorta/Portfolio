#include "FluidNarratorData.h"

#include "Engine/DataTable.h"

/*=============================================================================
Fluid Narrator Data Localization : Import Export
=============================================================================*/

void UFluidNarratorLocalization::ExportImportCreateDirectories()
{
	//Get Platform File Manager | Project Directories
	IPlatformFile& PlatformFileManager = FPlatformFileManager::Get().GetPlatformFile();
	const FString ProjectDirectory = FPaths::ProjectDir();
	
	//Import
	if(!PlatformFileManager.DirectoryExists(*(ProjectDirectory + "/ExportsImports/FluidNarrator/Import/")))
		PlatformFileManager.CreateDirectory(*(ProjectDirectory + "/ExportsImports/FluidNarrator/Import/"));
}

void UFluidNarratorLocalization::FilesExportJson()
{
	ExportImportCreateDirectories();

	//Get Platform File Manager
	IPlatformFile& PlatformFileManager = FPlatformFileManager::Get().GetPlatformFile();
	
	FFluidNarratorLocalizationImportExportJson ExportJsonTranslated;
	FFluidNarratorLocalizationImportExportJson ExportJsonNotTranslated;
	FFluidNarratorLocalizationTextImportExportJsonElement ExportText;

	//Export Texts Per Cultures
	TArray<FFluidNarratorLocalizationCulture> CultureArray;
	Cultures.GenerateValueArray(CultureArray);
	for(const FFluidNarratorLocalizationCulture& Culture : CultureArray)
	{
		//Do Not Export Out Native Culture
		if (Culture.bIsNativeCulture)
			continue;

		//Setup Json Objects
		ExportJsonTranslated.Copyright = Copyright.ToString();
		ExportJsonTranslated.License = License.ToString();
		ExportJsonTranslated.Language = Culture.Language.ToString();
		ExportJsonTranslated.NativeName = Culture.NativeName.ToString();
		ExportJsonTranslated.LanguageCode = Culture.LanguageCode.ToString();
		ExportJsonTranslated.PreviousTranslator = Culture.TranslatorAgency.ToString();
		ExportJsonTranslated.NewTranslator = "TRANSLATOR AGENCY";
		ExportJsonTranslated.PreviousTranslator = Culture.Translator.ToString();
		ExportJsonTranslated.NewTranslator = "TRANSLATOR";
		ExportJsonTranslated.PreviousVersion = Culture.Version.ToString();
		ExportJsonTranslated.NewVersion = "VERSION";
		ExportJsonTranslated.PreviousDate = Culture.Date.ToString();
		ExportJsonTranslated.NewDate = "DATE";
		ExportJsonTranslated.Texts.Empty();
		ExportJsonNotTranslated = ExportJsonTranslated;
		
		//Get Texts
		for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
			if (Pair.Key.Culture == Culture.Language)
			{
				//Translated
				ExportText.Namespace = Pair.Key.Namespace.ToString();
				ExportText.Source = Pair.Key.Source.ToString();
				ExportText.Translation = Pair.Value.Translation.ToString();
				ExportJsonTranslated.Texts.Add(ExportText);

				//Not Translated
				ExportText.Translation = "None";
				ExportJsonNotTranslated.Texts.Add(ExportText);
			}

		//Write Json File Text (With Translation)
		{
			//Create Json Root Object
			auto JsonObject = FJsonObjectConverter::UStructToJsonObject(ExportJsonTranslated).ToSharedRef();
			FString JsonFileText = "";
			
			//Write Json To File
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonFileText);
			if (FJsonSerializer::Serialize(JsonObject, Writer))
			{
				const FString Directory = FPaths::ProjectDir() + "/ExportsImports/FluidNarrator/Export/Json_Translated/texts_" + Culture.Language.ToString().ToLower() + ".json";
				FFileHelper::SaveStringToFile(JsonFileText,*Directory);
			}
		}

		//Write Json File Text (With No Translation)
		{
			//Create Json Root Object
			auto JsonObject = FJsonObjectConverter::UStructToJsonObject(ExportJsonNotTranslated).ToSharedRef();
			FString JsonFileText = "";
			
			//Write Json To File
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonFileText);
			if (FJsonSerializer::Serialize(JsonObject, Writer))
			{
				const FString Directory = FPaths::ProjectDir() + "/ExportsImports/FluidNarrator/Export/Json_NotTranslated/texts_" + Culture.Language.ToString().ToLower() + ".json";
				if (PlatformFileManager.FileExists(*Directory)) PlatformFileManager.DeleteFile(*Directory);
				FFileHelper::SaveStringToFile(JsonFileText,*Directory);
			}
		}
	}
}

void UFluidNarratorLocalization::FilesImportJson()
{
	ExportImportCreateDirectories();
	
	//Get Platform File Manager | Get Import Directory If Valid
	IPlatformFile& PlatformFileManager = FPlatformFileManager::Get().GetPlatformFile();
	const FString ImportDirectory = FPaths::ProjectDir() + "/ExportsImports/FluidNarrator/Import";
	if (!PlatformFileManager.DirectoryExists(*ImportDirectory))
		return;

	//Create Localization Text Pair | Localization Text
	FFluidNarratorLocalizationTextPair LocalizationTextPair = FFluidNarratorLocalizationTextPair();
	FFluidNarratorLocalizationText LocalizationText = FFluidNarratorLocalizationText();
	
	//Get Import Json Files
	TArray<FString> Files;
	const FString FileExtension = "json";
	PlatformFileManager.FindFiles(Files,*ImportDirectory,*FileExtension);

	//Import Json
	FFluidNarratorLocalizationImportExportJson Import;
	FString FileStringData = "";
	for(const FString& File : Files)
	{
		//Create Import Json Structure From File
		FFileHelper::LoadFileToString(FileStringData,*File);
		FJsonObjectConverter::JsonObjectStringToUStruct(FileStringData,&Import,0,0,false);

		//Only Load File If Culture Is Valid
		if (!Cultures.Contains(FName(Import.Language)))
			continue;

		//Set Culture Data
		Cultures[FName(Import.Language)].Language = FName(Import.Language);
		Cultures[FName(Import.Language)].NativeName = FName(Import.NativeName);
		Cultures[FName(Import.Language)].LanguageCode = FName(Import.LanguageCode);
		Cultures[FName(Import.Language)].TranslatorAgency = FName(Import.NewTranslatorAgency);
		Cultures[FName(Import.Language)].Translator = FName(Import.NewTranslator);
		Cultures[FName(Import.Language)].Version = FName(Import.NewVersion);
		Cultures[FName(Import.Language)].Date = FName(Import.NewDate);
		
		//SEt Localization Text Pair Culture
		LocalizationTextPair.Culture = FName(Import.Language);
	
		//Load Texts Into Asset's Texts
		for(const FFluidNarratorLocalizationTextImportExportJsonElement& Text : Import.Texts)
		{
			//Set Text
			LocalizationTextPair.Namespace = FName(Text.Namespace);
			LocalizationTextPair.Source = FName(Text.Source);
			LocalizationText.Translation = FName(Text.Translation);

			//Add Text
			if (Texts.Contains(LocalizationTextPair))
				Texts[LocalizationTextPair] = LocalizationText;
			else
				Texts.Add(LocalizationTextPair,LocalizationText);
		}
	}
}

void UFluidNarratorLocalization::FilesExportTsv()
{
	ExportImportCreateDirectories();

	//Get Platform File Manager
	IPlatformFile& PlatformFileManager = FPlatformFileManager::Get().GetPlatformFile();
	
	TArray<FFluidNarratorLocalizationTextImportExportJsonElement> ExportTexts;
	FFluidNarratorLocalizationTextImportExportJsonElement ExportText = FFluidNarratorLocalizationTextImportExportJsonElement();

	//Export Texts Per Cultures
	TArray<FFluidNarratorLocalizationCulture> CultureArray;
	Cultures.GenerateValueArray(CultureArray);
	for(const FFluidNarratorLocalizationCulture& Culture : CultureArray)
	{
		//Do Not Export Out Native Culture
		if (Culture.bIsNativeCulture)
			continue;
		
		//Empty Export Texts
		ExportTexts.Empty();

		//Get Texts
		for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
			if (Pair.Key.Culture == Culture.Language)
			{
				//Export Base (Add Quotes To Meet Tsv file Format requirements)
				ExportText.Namespace = Pair.Key.Namespace.ToString();
				ExportText.Source = Pair.Key.Source.ToString();
				ExportText.Translation = Pair.Value.Translation.ToString();
				ExportTexts.Add(ExportText);
			}

		//Write Tsv File Text (With Translation)
		FString TsvFileText = "";
		{
			TsvFileText += Copyright.ToString();
			TsvFileText += "\n" + License.ToString();
			TsvFileText += "\nLanguage:" + Culture.Language.ToString();
			TsvFileText += "\nNative-Name:" + Culture.NativeName.ToString();
			TsvFileText += "\nLanguage-Code:" + Culture.LanguageCode.ToString();
			TsvFileText += "\nPrevious-Translator-Agency:" + Culture.TranslatorAgency.ToString();
			TsvFileText += "\nNew-Translator-Agency: TRANSLATOR AGENCY";
			TsvFileText += "\nPrevious-Translator:" + Culture.Translator.ToString();
			TsvFileText += "\nNew-Translator: TRANSLATOR";
			TsvFileText += "\nPrevious-Version:" + Culture.Version.ToString();
			TsvFileText += "\nNew-Version:VERSION";
			TsvFileText += "\nPrevious-Date:" + Culture.Date.ToString();
			TsvFileText += "\nNew-Date: DATE";
			for (int i = 0; i < ExportTexts.Num(); i++)
				TsvFileText += "\n" + ExportTexts[i].Namespace + "\t" + ExportTexts[i].Source+ "\t" + ExportTexts[i].Translation;
			
			const FString Directory = FPaths::ProjectDir() + "/ExportsImports/FluidNarrator/Export/Tsv_Translated/texts_" + Culture.Language.ToString().ToLower() + ".tsv";
			FFileHelper::SaveStringToFile(TsvFileText,*Directory);
		}

		//Write Tsv File Text (With No Translation)
		TsvFileText = "";
		{
			TsvFileText += Copyright.ToString();
			TsvFileText += "\n" + License.ToString();
			TsvFileText += "\nLanguage:" + Culture.Language.ToString();
			TsvFileText += "\nNative-Name:" + Culture.NativeName.ToString();
			TsvFileText += "\nLanguage-Code:" + Culture.LanguageCode.ToString();
			TsvFileText += "\nPrevious-Translator-Agency:NONE";
			TsvFileText += "\nNew-Translator-Agency: TRANSLATOR AGENCY";
			TsvFileText += "\nPrevious-Translator:NONE";
			TsvFileText += "\nNew-Translator: TRANSLATOR";
			TsvFileText += "\nPrevious-Version:NONE";
			TsvFileText += "\nNew-Version:VERSION";
			TsvFileText += "\nPrevious-Date:NONE";
			TsvFileText += "\nNew-Date: DATE";
			for (int i = 0; i < ExportTexts.Num(); i++)
				TsvFileText += "\n" + ExportTexts[i].Namespace + "\t" + ExportTexts[i].Source+ "\t" + "None";
			
			const FString Directory = FPaths::ProjectDir() + "/ExportsImports/FluidNarrator/Export/Tsv_NotTranslated/texts_" + Culture.Language.ToString().ToLower() + ".tsv";
			if (PlatformFileManager.FileExists(*Directory)) PlatformFileManager.DeleteFile(*Directory);
			FFileHelper::SaveStringToFile(TsvFileText,*Directory);
		}
	}
}

void UFluidNarratorLocalization::FilesImportTsv()
{
	ExportImportCreateDirectories();

	//Get Platform File Manager | Get Import Directory If Valid
	IPlatformFile& PlatformFileManager = FPlatformFileManager::Get().GetPlatformFile();
	const FString ImportDirectory = FPaths::ProjectDir() + "/ExportsImports/FluidNarrator/Import";
	if (!PlatformFileManager.DirectoryExists(*ImportDirectory))
		return;

	//Create Localization Text Pair | Localization Text
	FFluidNarratorLocalizationTextPair LocalizationTextPair = FFluidNarratorLocalizationTextPair();
	FFluidNarratorLocalizationText LocalizationText = FFluidNarratorLocalizationText();
	
	//Get Import Tsv Files
	TArray<FString> Files;
	const FString FileExtension = "tsv";
	PlatformFileManager.FindFiles(Files,*ImportDirectory,*FileExtension);
	
	//Import Json
	FString FileData = "";
	TArray<FString> FileDataLines;
	TArray<FString> FileDataLineSplit;
	bool bIsValidCulture = false;
	for(const FString& File : Files)
	{
		//Get File String Data Lines
		FFileHelper::LoadFileToString(FileData,*File);
		FileData.ParseIntoArray(FileDataLines,TEXT("\n"), false);
		bIsValidCulture = false;
		
		for (FString Line : FileDataLines)
		{
			//Get Language
			if (Line.Contains("Language:"))
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT(":"), false);
				Line = FileDataLineSplit[1];
				Line.ReplaceInline(TEXT(","), TEXT(""),ESearchCase::IgnoreCase);
				Line.ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
				Line.TrimQuotesInline();
				Line.TrimStartAndEndInline();

				if (Cultures.Contains(FName(Line)))
				{
					LocalizationTextPair.Culture = FName(Line);
					Cultures[FName(Line)].Language = FName(Line);
					bIsValidCulture = true;
				}
				else
				{
					break;
				}
			}

			//Get Native Name
			else if (bIsValidCulture && Line.Contains("Native-Name:"))
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT(":"), false);
				Line = FileDataLineSplit[1];
				Line.ReplaceInline(TEXT(","), TEXT(""),ESearchCase::IgnoreCase);
				Line.ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
				Line.TrimQuotesInline();
				Line.TrimStartAndEndInline();
				Cultures[LocalizationTextPair.Culture].NativeName = FName(Line);
			}
			
			//Get Language Code
			else if (bIsValidCulture && Line.Contains("Language-Code:"))
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT(":"), false);
				Line = FileDataLineSplit[1];
				Line.ReplaceInline(TEXT(","), TEXT(""),ESearchCase::IgnoreCase);
				Line.ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
				Line.TrimQuotesInline();
				Line.TrimStartAndEndInline();
				Cultures[LocalizationTextPair.Culture].LanguageCode = FName(Line);
			}

			//Get New Translator
			else if (bIsValidCulture && Line.Contains("New-Translator-Agency:"))
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT(":"), false);
				Line = FileDataLineSplit[1];
				Line.ReplaceInline(TEXT(","), TEXT(""),ESearchCase::IgnoreCase);
				Line.ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
				Line.TrimQuotesInline();
				Line.TrimStartAndEndInline();
				Cultures[LocalizationTextPair.Culture].TranslatorAgency = FName(Line);
			}
			
			//Get New Translator
			else if (bIsValidCulture && Line.Contains("New-Translator:"))
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT(":"), false);
				Line = FileDataLineSplit[1];
				Line.ReplaceInline(TEXT(","), TEXT(""),ESearchCase::IgnoreCase);
				Line.ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
				Line.TrimQuotesInline();
				Line.TrimStartAndEndInline();
				Cultures[LocalizationTextPair.Culture].Translator = FName(Line);
			}
			
			//Get New Version
			else if (bIsValidCulture && Line.Contains("New-Version:"))
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT(":"), false);
				Line = FileDataLineSplit[1];
				Line.ReplaceInline(TEXT(","), TEXT(""),ESearchCase::IgnoreCase);
				Line.ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
				Line.TrimQuotesInline();
				Line.TrimStartAndEndInline();
				Cultures[LocalizationTextPair.Culture].Version = FName(Line);
			}

			//Get New Date
			else if (bIsValidCulture && Line.Contains("New-Date:"))
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT(":"), false);
				Line = FileDataLineSplit[1];
				Line.ReplaceInline(TEXT(","), TEXT(""),ESearchCase::IgnoreCase);
				Line.ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
				Line.TrimQuotesInline();
				Line.TrimStartAndEndInline();
				Cultures[LocalizationTextPair.Culture].Date = FName(Line);
			}
			
			//Get Localization Text
			else if (bIsValidCulture)
			{
				Line.ParseIntoArray(FileDataLineSplit,TEXT("\t"), false);
				if (FileDataLineSplit.Num() >= 3)
				{
					FileDataLineSplit[0].ReplaceInline(TEXT("\n"), TEXT(""),ESearchCase::IgnoreCase);
					FileDataLineSplit[0].ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
					LocalizationTextPair.Namespace = FName(FileDataLineSplit[0]);
					FileDataLineSplit[1].ReplaceInline(TEXT("\n"), TEXT(""),ESearchCase::IgnoreCase);
					FileDataLineSplit[1].ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
					LocalizationTextPair.Source = FName(FileDataLineSplit[1]);
					FileDataLineSplit[2].ReplaceInline(TEXT("\n"), TEXT(""),ESearchCase::IgnoreCase);
					FileDataLineSplit[2].ReplaceInline(TEXT("\r"), TEXT(""),ESearchCase::IgnoreCase);
					LocalizationText.Translation = FName(FileDataLineSplit[2]);

					//Add Text
					if (Texts.Contains(LocalizationTextPair))
						Texts[LocalizationTextPair] = LocalizationText;
					else
						Texts.Add(LocalizationTextPair,LocalizationText);
				}
			}
		}
	}
}

/*=============================================================================
Fluid Narrator Data Localization : Get Set Localization Texts	
=============================================================================*/

void UFluidNarratorLocalization::SetLocalizationTexts(const TArray<FName> NamespacesIn, const TArray<FName> CulturesIn, const TArray<FString> SourcesIn, const TArray<FString> TranslationsIn, const TArray<bool> bAutoTranslatedsIn)
{
	//Empty
	Texts.Empty();

	//Set Namespace Culture Texts
	for (int i = 0; i < NamespacesIn.Num() && i < CulturesIn.Num() && i < SourcesIn.Num() && i < TranslationsIn.Num() && i < bAutoTranslatedsIn.Num(); i++)
	{
		//Set Pair
		FFluidNarratorLocalizationTextPair Pair;
		Pair.Namespace = NamespacesIn[i];
		Pair.Culture = CulturesIn[i];
		Pair.Source = FName(SourcesIn[i]);

		//Add Namespace Culture Pair
		if (!Texts.Contains(Pair))
			Texts.Add(Pair,FFluidNarratorLocalizationText());

		//Add Source Translation Pair To Namespace Culture Pair
		Texts[Pair].Translation = FName(TranslationsIn[i]);
		Texts[Pair].bIsAutoTranslated = bAutoTranslatedsIn[i];
	}
}

void UFluidNarratorLocalization::GetLocalizationTexts(TArray<FName>& Namespaces_Out, TArray<FName>& Cultures_Out, TArray<FString>& Sources_Out, TArray<FString>& Translations_Out, TArray<bool>& bAutoTranslateds_Out)
{
	//Get
	for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
	{
		Namespaces_Out.Add(FName(Pair.Key.Namespace));
		Cultures_Out.Add(FName(Pair.Key.Culture));
		Sources_Out.Add(Pair.Key.Source.ToString());
		Translations_Out.Add(Pair.Value.Translation.ToString());
		bAutoTranslateds_Out.Add(Pair.Value.bIsAutoTranslated);
	}
}

void UFluidNarratorLocalization::GetLocalizationTextAsString(const FName CultureIn, const FString StringIn, FString& String_Out)
{
	//Set Default Outs
	String_Out = StringIn;

	//Get Localization
	for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
		if (Pair.Key.Culture == CultureIn && Pair.Key.Source.ToString() == StringIn)
		{
			String_Out = Pair.Value.Translation.ToString();
			return;
		}
}

void UFluidNarratorLocalization::GetLocalizationTextAsText(const FName CultureIn, const FText TextIn, FText& Text_Out)
{
	//Set Default Outs
	Text_Out = TextIn;

	//Get Localization
	for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
		if (Pair.Key.Culture == CultureIn && Pair.Key.Source.ToString() == TextIn.ToString())
		{
			Text_Out = FText::FromString(Pair.Value.Translation.ToString());
			return;
		}
}

void UFluidNarratorLocalization::GetLocalizationTextAsTexts(const FName CultureIn, const TArray<FText> TextsIn, TArray<FText>& Texts_Out)
{
	//Set Default Outs
	Texts_Out = TextsIn;

	//Get Localization
	for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
	{
		for (int i = 0; i < TextsIn.Num(); i++)
		{
			if (Pair.Key.Culture == CultureIn && Pair.Key.Source.ToString() == TextsIn[i].ToString())
				Texts_Out.Add(FText::FromString(Pair.Value.Translation.ToString()));
		}
	}
}

void UFluidNarratorLocalization::GetLocalizationTextAsName(const FName CultureIn, const FName NameIn, FName& Name_Out)
{
	//Set Default Outs
	Name_Out = NameIn;

	//Get Localization
	for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
		if (Pair.Key.Culture == CultureIn && Pair.Key.Source == NameIn)
		{
			Name_Out = Pair.Value.Translation;
			return;
		}
}

void UFluidNarratorLocalization::GetLocalizationTextAsNames(const FName CultureIn, const TArray<FName> NamesIn, TArray<FName>& Names_Out)
{
	//Set Default Outs
	Names_Out = NamesIn;

	//Get Localization
	for (const TPair<FFluidNarratorLocalizationTextPair,FFluidNarratorLocalizationText>& Pair : Texts)
	{
		for (int i = 0; i < NamesIn.Num(); i++)
		{
			if (Pair.Key.Culture == CultureIn && Pair.Key.Source == NamesIn[i])
				Names_Out.Add(Pair.Value.Translation);
		}
	}
}