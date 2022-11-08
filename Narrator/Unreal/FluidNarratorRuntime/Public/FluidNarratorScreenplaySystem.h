#pragma once

#include <queue>
#include "FluidNarratorRuntime.h"
#include "CoreMinimal.h"

/**
 * 
 */

class UFluidNarratorLocalization;
class SFluidNarratorScreenplaySlateWidget;
class UFluidNarratorScreenplayUserWidget;

//Max Entities | Component Count
const int FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX = 25;

//Fluid Narrator Screenplay Controller Condition Check Results
enum class FLUIDNARRATORRUNTIME_API EFluidNarratorScreenplaySystemConditionCheckResults : uint8
{
	Passed,
	Failed,
	NoCheckNeeded,
};

struct FluidNarratorScreenplaySystemAutoResponseComponent
{
	FName CharacterID;
	FString TextAsString;
	uint8 ResponseIndex;
	float TimerCurrent;
	float TimerStart;
	bool bEnabled;

	FluidNarratorScreenplaySystemAutoResponseComponent()
	{
		CharacterID = FName("");
		TextAsString = "";
		ResponseIndex = 0;
		TimerCurrent = 0;
		TimerStart = 0;
		bEnabled = false;
	}
};

class FLUIDNARRATORRUNTIME_API FluidNarratorScreenplaySystemSyntax
{
public:
	
	static FString Comment;
	static FString Definition;
	static FString Namespace;
	static FString Variable;
	static FString VariableCheck;
	static FString VariableSet;
	static FString Section;
	static FString SectionGoTo;
	static FString Separator;
	static FString SelectorStart;
	static FString SelectorEnd;
	static FString Event;
	static FString Text;
	static FString Response;
	static FString ResponseDefault;
	static FString TextDefaultCharacterID;
	static FString ResponseDefaultCharacterID;
};

//Fluid Narrator Screenplay System
class FLUIDNARRATORRUNTIME_API FluidNarratorScreenplaySystem
{
//Object	
public:

	FluidNarratorScreenplaySystem();
	
	UFluidNarratorLocalization* Localization;

//Task Start Stop
public:
	
	void StartBySectionName(const FName Culture, const FString TextAsString, const FName SectionName, UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget, SFluidNarratorScreenplaySlateWidget* ScreenplaySlateWidget, int& ScreenplayTask_Out);
	void StartBySectionAuto(const FName Culture, const FString TextAsString, UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget, SFluidNarratorScreenplaySlateWidget* ScreenplaySlateWidget, int& ScreenplayTask_Out);
	void Stop(const int ScreenplayTask, int& ScreenplayTask_Out);
	void StopAutoResponse(const int ScreenplayTask, int& ScreenplayTask_Out);
	void GetTaskCount(int& TaskCount_Out);
	void LoadInVariablesAtHeader(const FString TextAsString);
	std::queue<int> TasksAvailable;
	int TaskCount;

//Tick
public:
	
	void Tick(float DeltaTime);
	FString GetCurrentLineText(const int ScreenplayTask);

//Parse
public:

	static void ParseDefinitionText(const FString String, FString& String_Out);
	static void ParseNamespaceText(const FString String, FString& String_Out);
	static void ParseGetTextsAndResponsesCombined(const FString String, TArray<FString>& Strings_Out);
	static void ParseVariablesFromString(const FString VariablesAsString, TMap<FName,FString>& VariablesMap_Out);
	static void ParseVariablesToString(const TMap<FName,FString> VariablesMap, FString& VariablesAsString_Out);
	static void ParseVariablesApplyToString(const FString String, const TMap<FName,FString> Variables, FString& String_Out);
	static void ParseVariablesMetCheck(const FString String, const TMap<FName,FString> Variables, EFluidNarratorScreenplaySystemConditionCheckResults& Results_Out);
	static void ParseVariablesSet(const FString String, TMap<FName,FString> Variables,TMap<FName,FString>& Variables_Out);
	static void ParseCharacterID(const FString String, const FName CharacterIDDefault, FString& String_Out, FName& CharacterID_Out);
	static void ParseSelectorResponse(const FString String, const int ResponseIndex,  FString& String_Out);
	static void ParseSplitStringAndGetRandom(const FString String, const FString SplitBy, FString& String_Out);
	static void ParseSplitStringAndGetAll(const FString String, const FString SplitBy, TArray<FString>& Strings_Out);
	static void ParseSplitStringAndGetFirst(const FString String, const FString SplitBy, FString& String_Out);
	static void ParseSplitStringUpToSpace(const FString String, FString& String_Out);
	
//Task Data
public:

	TMap<FName,FString> Variables;
	SFluidNarratorScreenplaySlateWidget* TaskSlateWidgets[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	UFluidNarratorScreenplayUserWidget* TaskUserWidgets[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	TArray<FString> TaskLines[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	FName TaskDefaultTextCharacterIDs[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	FName TaskDefaultResponseCharacterIDs[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	int TaskResponseIndexs[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	int TaskTextLinesIndexNexts[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	int TaskTextLinesIndexCurrents[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	bool bTaskIsRunnings[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	bool bTaskIsWaitingForTextAndResponses[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	bool bTaskIsWaitingForEvents[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	bool bTaskConditionsMets[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];

	FName TaskCultures[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
	FluidNarratorScreenplaySystemAutoResponseComponent TasksAutoResponseComponents[FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX];
};
