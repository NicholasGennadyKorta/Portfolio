#include "FluidNarratorScreenplaySystem.h"

/*=============================================================================
Fluid Narrator Screenplay Interpreter Syntax
=============================================================================*/

//Token Texts
FString FluidNarratorScreenplaySystemSyntax::Comment = "/";
FString FluidNarratorScreenplaySystemSyntax::Definition = "~";
FString FluidNarratorScreenplaySystemSyntax::Namespace = "@";
FString FluidNarratorScreenplaySystemSyntax::Variable = "$";
FString FluidNarratorScreenplaySystemSyntax::VariableCheck = "$?";
FString FluidNarratorScreenplaySystemSyntax::VariableSet = "$!";
FString FluidNarratorScreenplaySystemSyntax::Section = "--";
FString FluidNarratorScreenplaySystemSyntax::SectionGoTo = "->";
FString FluidNarratorScreenplaySystemSyntax::Separator = "|";
FString FluidNarratorScreenplaySystemSyntax::SelectorStart = "[";
FString FluidNarratorScreenplaySystemSyntax::SelectorEnd = "]";
FString FluidNarratorScreenplaySystemSyntax::Event = "#";
FString FluidNarratorScreenplaySystemSyntax::Text = "*";
FString FluidNarratorScreenplaySystemSyntax::Response = "**";
FString FluidNarratorScreenplaySystemSyntax::ResponseDefault = "***";
FString FluidNarratorScreenplaySystemSyntax::TextDefaultCharacterID = ":*";
FString FluidNarratorScreenplaySystemSyntax::ResponseDefaultCharacterID = ":**";

/*=============================================================================
Fluid Narrator Screenplay System : Constructor
=============================================================================*/

FluidNarratorScreenplaySystem::FluidNarratorScreenplaySystem()
{
	Variables.Empty();
	
	// Initialize Tasks Available Queue With All Possible Tasks
	for (int i = 0; i < FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX; ++i)
	{
		TasksAvailable.push(i);

		//Default Task Values
		TaskSlateWidgets[i] = nullptr;
		TaskUserWidgets[i] = nullptr;
		TaskLines[i].Empty();
		TaskDefaultTextCharacterIDs[i] = FName();
		TaskDefaultResponseCharacterIDs[i] = FName();
		TaskTextLinesIndexNexts[i] = 0;
		TaskTextLinesIndexCurrents[i] = 0;
		TaskResponseIndexs[i] = 0;
		bTaskIsRunnings[i] = false;
		bTaskIsWaitingForTextAndResponses[i] = false;
		bTaskIsWaitingForEvents[i] = false;
		bTaskConditionsMets[i] = true;
		TaskCultures[i] = "";
		TasksAutoResponseComponents[i] = FluidNarratorScreenplaySystemAutoResponseComponent();
	}
	TaskCount = 0;
}

/*=============================================================================
Fluid Narrator Screenplay System : Task Start Stop
=============================================================================*/

void FluidNarratorScreenplaySystem::StartBySectionName(const FName Culture, FString TextAsString, const FName SectionName, UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget, SFluidNarratorScreenplaySlateWidget* ScreenplaySlateWidget, int& ScreenplayTask_Out)
{
	//Return If We Reached Max Task Count
	if (TaskCount >= FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX)
	{
		ScreenplayTask_Out = -1;
		return;
	}
	
	//Take A Task From The Front Of Available Queue
	ScreenplayTask_Out = TasksAvailable.front();
	TasksAvailable.pop();
	++TaskCount;

	//Set Screenplay User Widget
	if (ScreenplayUserWidget)
	{
		ScreenplayUserWidget->ScreenplayTask = ScreenplayTask_Out;
		ScreenplayUserWidget->ScreenplaySystem = this;
		ScreenplayUserWidget->OnStartPre();
	}
	TaskUserWidgets[ScreenplayTask_Out] = ScreenplayUserWidget;
	
	//Set Screenplay Slate Widget
	if (ScreenplaySlateWidget)
	{
		ScreenplaySlateWidget->ScreenplayTask = ScreenplayTask_Out;
		ScreenplaySlateWidget->ScreenplaySystem = this;
		ScreenplaySlateWidget->OnStartPre();
	}
	TaskSlateWidgets[ScreenplayTask_Out] = ScreenplaySlateWidget;

	//Load In Variables At Header
	LoadInVariablesAtHeader(TextAsString);
	
	//Parse Text Into Lines
	TextAsString.ParseIntoArray(TaskLines[ScreenplayTask_Out], TEXT("\n"), false);

	//Find Section
	FString LineToCheck = "";
	bool bFound = false;
	for (int i = 0; i < TaskLines[ScreenplayTask_Out].Num(); i++)
	{
		if (TaskLines[ScreenplayTask_Out][i].StartsWith(FluidNarratorScreenplaySystemSyntax::Section))
		{
			LineToCheck = TaskLines[ScreenplayTask_Out][i];
			LineToCheck.ReplaceInline(TEXT("-"), TEXT(""),ESearchCase::IgnoreCase); //Get Rid Of All -
			ParseSplitStringAndGetFirst(LineToCheck,"(",LineToCheck); //Get Rid Of Variables
						
			//Found Section
			if (LineToCheck == SectionName.ToString().ToLower())
			{
				TaskCultures[ScreenplayTask_Out] = Culture;
				TaskTextLinesIndexNexts[ScreenplayTask_Out] = i;
				bTaskIsRunnings[ScreenplayTask_Out] = true;
				
				//On Start
				if (ScreenplayUserWidget || ScreenplaySlateWidget)
				{
					FString Line = TaskLines[ScreenplayTask_Out][TaskTextLinesIndexNexts[ScreenplayTask_Out]];
					const FText LineAsText = FText::FromString(Line);
					const FName LineAsName = FName(Line);
					const int LineStartIndex = TaskTextLinesIndexNexts[ScreenplayTask_Out];
					
					if (ScreenplaySlateWidget)
						ScreenplaySlateWidget->OnStart(FText::FromString(Line),LineStartIndex);
					if (ScreenplayUserWidget)
						ScreenplayUserWidget->OnStart(LineAsText,LineStartIndex);

					//Line
					ParseSplitStringAndGetFirst(Line,"(",Line);
					Line.ReplaceInline(TEXT("-"),TEXT(""),ESearchCase::IgnoreCase);
					
					if (ScreenplaySlateWidget)
						ScreenplaySlateWidget->OnSectionStart(FName(Line));
					if (ScreenplayUserWidget)
						ScreenplayUserWidget->OnSectionStart(LineAsName);
				}

				//Go To Next Line Off The Bat
				TaskTextLinesIndexNexts[ScreenplayTask_Out] = i + 1;
				TaskTextLinesIndexCurrents[ScreenplayTask_Out] = TaskTextLinesIndexNexts[ScreenplayTask_Out];

				//Return, We Are Successful
				return;
			}
		}
	}

	//Stop If No Section Found
	if (!bFound)
		Stop(ScreenplayTask_Out,ScreenplayTask_Out);

	
}

void FluidNarratorScreenplaySystem::StartBySectionAuto(const FName Culture, FString TextAsString, UFluidNarratorScreenplayUserWidget* ScreenplayUserWidget, SFluidNarratorScreenplaySlateWidget* ScreenplaySlateWidget, int& ScreenplayTask_Out)
{
	//Return If We Reached Max Task Count
	if (TaskCount >= FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX)
	{
		ScreenplayTask_Out = -1;
		return;
	}
	
	//Take A Task From The Front Of Available Queue
	ScreenplayTask_Out = TasksAvailable.front();
	TasksAvailable.pop();
	++TaskCount;

	//Set Screenplay User Widget
	if (ScreenplayUserWidget)
	{
		ScreenplayUserWidget->ScreenplayTask = ScreenplayTask_Out;
		ScreenplayUserWidget->ScreenplaySystem = this;
		ScreenplayUserWidget->OnStartPre();
	}
	TaskUserWidgets[ScreenplayTask_Out] = ScreenplayUserWidget;
	
	//Set Screenplay Slate Widget
	if (ScreenplaySlateWidget)
	{
		ScreenplaySlateWidget->ScreenplayTask = ScreenplayTask_Out;
		ScreenplaySlateWidget->ScreenplaySystem = this;
		ScreenplaySlateWidget->OnStartPre();
	}
	TaskSlateWidgets[ScreenplayTask_Out] = ScreenplaySlateWidget;

	//Load In Variables At Header
	LoadInVariablesAtHeader(TextAsString);
	
	//Parse Text Into Lines
	TextAsString.ParseIntoArray(TaskLines[ScreenplayTask_Out], TEXT("\n"), false);
	
	//Find Section To Start With
	for (int i = 0; i <TaskLines[ScreenplayTask_Out].Num() ; i++)
	{
		if (TaskLines[ScreenplayTask_Out][i].StartsWith(FluidNarratorScreenplaySystemSyntax::Section))
		{
			EFluidNarratorScreenplaySystemConditionCheckResults Result;
			ParseVariablesMetCheck(TaskLines[ScreenplayTask_Out][i],Variables,Result);
			if (Result != EFluidNarratorScreenplaySystemConditionCheckResults::Failed)
			{
				TaskCultures[ScreenplayTask_Out] = Culture;
				TaskTextLinesIndexNexts[ScreenplayTask_Out] = i;
				bTaskIsRunnings[ScreenplayTask_Out] = true;
				
				//On Start
				if (ScreenplayUserWidget || ScreenplaySlateWidget)
				{
					FString Line = TaskLines[ScreenplayTask_Out][TaskTextLinesIndexNexts[ScreenplayTask_Out]];
					const FText LineAsText = FText::FromString(Line);
					const FName LineAsName = FName(Line);
					const int LineStartIndex = TaskTextLinesIndexNexts[ScreenplayTask_Out];
					
					if (ScreenplaySlateWidget)
						ScreenplaySlateWidget->OnStart(FText::FromString(Line),LineStartIndex);
					if (ScreenplayUserWidget)
						ScreenplayUserWidget->OnStart(LineAsText,LineStartIndex);

					//Line
					ParseSplitStringAndGetFirst(Line,"(",Line);
					Line.ReplaceInline(TEXT("-"),TEXT(""),ESearchCase::IgnoreCase);
					
					if (ScreenplaySlateWidget)
						ScreenplaySlateWidget->OnSectionStart(FName(Line));
					if (ScreenplayUserWidget)
						ScreenplayUserWidget->OnSectionStart(LineAsName);
				}

				//Go To Next Line Off The Bat
				TaskTextLinesIndexNexts[ScreenplayTask_Out] = i + 1;
				TaskTextLinesIndexCurrents[ScreenplayTask_Out] = TaskTextLinesIndexNexts[ScreenplayTask_Out];

				//Return, We Are Successful
				return;
			}
		}
	}

	//Not Successful So Stop
	Stop(ScreenplayTask_Out,ScreenplayTask_Out);

}

void FluidNarratorScreenplaySystem::Stop(const int ScreenplayTask, int& ScreenplayTask_Out)
{
	//Return If Is Not Valid
	if (ScreenplayTask < 0 || ScreenplayTask >= FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX)
	{
		ScreenplayTask_Out = -1;
		return;
	}
	
	//Only If Running
	if (bTaskIsRunnings[ScreenplayTask])
	{
		bTaskIsRunnings[ScreenplayTask] = false;
		bTaskIsWaitingForTextAndResponses[ScreenplayTask] = false;
		bTaskIsWaitingForEvents[ScreenplayTask] = false;
		TaskTextLinesIndexNexts[ScreenplayTask] = 0;
		TaskTextLinesIndexCurrents[ScreenplayTask] = 0;
		TaskResponseIndexs[ScreenplayTask] = 0;
		TaskDefaultTextCharacterIDs[ScreenplayTask] = FName();
		TaskDefaultResponseCharacterIDs[ScreenplayTask] = FName();
		bTaskConditionsMets[ScreenplayTask] = true;
		bTaskIsRunnings[ScreenplayTask] = false;
		TaskCultures[ScreenplayTask] = "";
		TasksAutoResponseComponents[ScreenplayTask] = FluidNarratorScreenplaySystemAutoResponseComponent();

		if (TaskUserWidgets[ScreenplayTask])
			TaskUserWidgets[ScreenplayTask]->OnStop();
		if (TaskSlateWidgets[ScreenplayTask])
			TaskSlateWidgets[ScreenplayTask]->OnStop();
		TaskUserWidgets[ScreenplayTask] = nullptr;
		TaskSlateWidgets[ScreenplayTask] = nullptr;
		
		// Put Destroyed Entity Back In Available Que (At The Back) | Decrease Entity Count
		TasksAvailable.push(ScreenplayTask);
		--TaskCount;

		ScreenplayTask_Out = -1;
	}
}

void FluidNarratorScreenplaySystem::StopAutoResponse(const int ScreenplayTask, int& ScreenplayTask_Out)
{
	//Return If Is Not Valid
	if (ScreenplayTask < 0 || ScreenplayTask >= FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX)
	{
		ScreenplayTask_Out = -1;
		return;
	}
	
	ScreenplayTask_Out = ScreenplayTask;
	TasksAutoResponseComponents[ScreenplayTask] = FluidNarratorScreenplaySystemAutoResponseComponent();
}

void FluidNarratorScreenplaySystem::GetTaskCount(int& TaskCount_Out)
{
	TaskCount_Out = TaskCount;	
}

void FluidNarratorScreenplaySystem::LoadInVariablesAtHeader(const FString TextAsString)
{
	//Parse Input Into Lines
	TArray<FString> Lines;
	TextAsString.ParseIntoArray(Lines, TEXT("\n"), false);
	
	//Read Variables ($)
	{
		//Read Variables
		for (int i = 0; i < Lines.Num() ; i++)
		{
			if (Lines[i].StartsWith(FluidNarratorScreenplaySystemSyntax::Variable))
			{
				//Get Line | Remove Spaces
				FString L = Lines[i];
				L.TrimStartAndEndInline();
				L.ReplaceInline(TEXT(" "), TEXT(""),ESearchCase::IgnoreCase);
			
				//Do Not Want To Check Inside Sections
				if (L.Contains(FluidNarratorScreenplaySystemSyntax::Section))
					break;

				//Add If Is Valid
				int I = 0;
				L.FindChar('=',I);
				if (I != INDEX_NONE)
				{
					FName VariableName = FName(L.Left(I));
					FString VariableValue = L.Right(L.Len() - 1 - I);
					if (!Variables.Contains(VariableName))
						Variables.Add(VariableName,VariableValue);
				}
			}
		}
	}
}

/*=============================================================================
Fluid Narrator Screenplay System  : Tick
=============================================================================*/

void FluidNarratorScreenplaySystem::Tick(float DeltaTime)
{
	//Chache Task Variables
	FString TaskLine = "";
	UFluidNarratorScreenplayUserWidget* TaskUserWidget = nullptr;
	SFluidNarratorScreenplaySlateWidget* TaskSlateWidget = nullptr;
	int Task = -1;
	int TaskLineIndexNext = 0;
	bool TaskConditionsMet = false;
	
	//Do Tasks
	for (int i = 0; i < FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX; i++)
	{
		//Get Task
		Task = i;
		
		//Auto Response
		if (TasksAutoResponseComponents[Task].bEnabled)
		{
			TasksAutoResponseComponents[Task].TimerCurrent -= DeltaTime;
			if (TasksAutoResponseComponents[Task].TimerCurrent <= 0)
			{
				TArray<FName> TextsCharacterIDs = { TasksAutoResponseComponents[Task].CharacterID };
				TArray<FName> ResponseCharacterIDs = {};
				TArray<FText>  Texts = { FText::FromString(TasksAutoResponseComponents[Task].TextAsString) };
				TArray<FText>  ResponseTexts = { };
				TArray<int>  ResponseIndexs = { };
				bool bHasAutoResponse = false;

				if (TasksAutoResponseComponents[Task].TextAsString != ".")
				{
					bTaskIsWaitingForTextAndResponses[Task] = true;
					if (TaskSlateWidgets[Task])
						TaskSlateWidgets[Task]->OnSetTextsAndResponses(TextsCharacterIDs,ResponseCharacterIDs,Texts,ResponseTexts,ResponseIndexs,bHasAutoResponse);
					if (TaskUserWidgets[Task])
						TaskUserWidgets[Task]->OnSetTextsAndResponses(TextsCharacterIDs,ResponseCharacterIDs,Texts,ResponseTexts,ResponseIndexs,bHasAutoResponse);
				}
				else
				{
					bTaskIsWaitingForTextAndResponses[Task] = false;
				}
				
				//Reset Auto Response
				TaskResponseIndexs[Task] = TasksAutoResponseComponents[Task].ResponseIndex;
				TasksAutoResponseComponents[Task] = FluidNarratorScreenplaySystemAutoResponseComponent();
			}
			continue;
		}
		
		//Skip If Not Running Or Waiting
		if (!bTaskIsRunnings[Task] ||  bTaskIsWaitingForEvents[Task] || bTaskIsWaitingForTextAndResponses[Task] || TaskLines[Task].Num() == 0 || TaskTextLinesIndexCurrents[Task] >= TaskLines[Task].Num())
			continue;

		//Get Task Data
		TaskLine = TaskLines[Task][TaskTextLinesIndexNexts[Task]];
		TaskUserWidget = TaskUserWidgets[Task];
		TaskSlateWidget = TaskSlateWidgets[Task];
		TaskLineIndexNext = TaskTextLinesIndexNexts[Task];
		TaskConditionsMet = bTaskConditionsMets[Task];
		
		//Parse Comments
		ParseSplitStringAndGetFirst(TaskLine,FluidNarratorScreenplaySystemSyntax::Comment,TaskLine);

		//As Empty Line
		if (TaskLine.StartsWith(" ") || TaskLine == "")
		{
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
			TaskTextLinesIndexNexts[Task]++;
			bTaskConditionsMets[Task] = true;
		}
		
		//Check Variables (Go To Next Line And Continue Checks)
		else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::VariableCheck))
		{
			TaskLine.RightChopInline(2);
			EFluidNarratorScreenplaySystemConditionCheckResults ConditionCheckResults;
			ParseVariablesMetCheck(TaskLine,Variables,ConditionCheckResults);
			ParseSelectorResponse(TaskLine,TaskResponseIndexs[Task],TaskLine);
			if (ConditionCheckResults == EFluidNarratorScreenplaySystemConditionCheckResults::Failed && TaskLine != "")
				bTaskConditionsMets[Task] = false;
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
			TaskTextLinesIndexNexts[Task]++;
		}

		//Set Variables
		else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::VariableSet))
		{
			ParseSelectorResponse(TaskLine,TaskResponseIndexs[Task],TaskLine);
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
			TaskTextLinesIndexNexts[Task]++;
			bTaskConditionsMets[Task] = true;
			if (TaskLine != "" && TaskConditionsMet)
				ParseVariablesSet(TaskLine,Variables,Variables);
		}
		
		//Go To Section
		else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::SectionGoTo))
		{
			//Get Rid Of ->
			TaskLine.RightChopInline(2);
			ParseSelectorResponse(TaskLine,TaskResponseIndexs[Task],TaskLine);

			if (TaskLine != "" && TaskConditionsMet)
			{
				FString LineToCheck = "";
				bool bFound = false;
				for (int j = 0; j < TaskLines[Task].Num(); j++)
				{
					if (TaskLines[Task][j].StartsWith(FluidNarratorScreenplaySystemSyntax::Section))
					{
						LineToCheck = TaskLines[Task][j];
						LineToCheck.ReplaceInline(TEXT("-"), TEXT(""),ESearchCase::IgnoreCase); //Get Rid Of All -
						ParseSplitStringAndGetFirst(LineToCheck,"(",LineToCheck); //Get Rid Of Variables
						FName LineToCheckAsName = FName(LineToCheck);
						
						//Found Section
						if (LineToCheck.ToLower() == TaskLine.ToLower())
						{
							TaskTextLinesIndexNexts[Task] = j + 1;
							TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
							bTaskConditionsMets[Task] = true;
							bFound = true;

							//On Section Start
							if (TaskUserWidget) TaskUserWidget->OnSectionStart(LineToCheckAsName);
							if (TaskSlateWidget) TaskSlateWidget->OnSectionStart(FName(LineToCheck));
							
							break;
						}
					}
				}

				//Stop If No Section Found (Continues To Next Task)
				if (!bFound)
					Stop(Task,Task);
			}

			//Not Doing So Go To Next Line
			else
			{
				TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
				TaskTextLinesIndexNexts[Task]++;
				bTaskConditionsMets[Task] = true;
			}
		}
		
		//As Event
		else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::Event))
		{
			TaskLine = TaskLine.RightChop(1);
			ParseVariablesApplyToString(TaskLine,Variables,TaskLine);
			ParseSplitStringUpToSpace(TaskLine,TaskLine);
			ParseSelectorResponse(TaskLine,TaskResponseIndexs[Task],TaskLine);
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
			TaskTextLinesIndexNexts[Task]++;
			bTaskConditionsMets[Task] = true;
			bTaskIsWaitingForEvents[Task] = true;
			if (TaskUserWidget && TaskConditionsMet && TaskLine != "")
				TaskUserWidget->OnEvent(TaskLine);
			if (TaskSlateWidget && TaskConditionsMet && TaskLine != "")
				TaskSlateWidget->OnEvent(TaskLine);
		}

		//Default Response Character ID
		else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::ResponseDefaultCharacterID))
		{
			ParseVariablesApplyToString(TaskLine,Variables,TaskLine);
			ParseSelectorResponse(TaskLine,TaskResponseIndexs[Task],TaskLine);
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
			TaskTextLinesIndexNexts[Task]++;
			bTaskConditionsMets[Task] = true;
			if (TaskConditionsMet && TaskLine != "")
				TaskDefaultResponseCharacterIDs[Task] = FName(TaskLine.RightChop(3));
		}
		
		//Default Text Character ID
		else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::TextDefaultCharacterID))
		{
			ParseVariablesApplyToString(TaskLine,Variables,TaskLine);
			ParseSelectorResponse(TaskLine,TaskResponseIndexs[Task],TaskLine);
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
			TaskTextLinesIndexNexts[Task]++;
			bTaskConditionsMets[Task] = true;
			if (TaskConditionsMet && TaskLine != "")
				TaskDefaultTextCharacterIDs[Task] = FName(*TaskLine.RightChop(2));
		}
		
		//As Text And Responses
		else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::Text))
		{
			//Cache Variables (Other)
			TArray<FName> NewTextsCharacterIDs;
			TArray<FName> NewResponsesCharacterIDs;
			TArray<FText> NewTexts;
			TArray<FText> NewResponses;
			TArray<int> NewResponseIndexs;

			FName NewCharacterID;
			int LineIter = 0;
			
			while (true)
			{
				if (TaskLineIndexNext + LineIter >= TaskLines[Task].Num())
					break;
				
				FString StringToWorkWith = TaskLines[Task][TaskLineIndexNext + LineIter];
				ParseSplitStringAndGetFirst(StringToWorkWith,FluidNarratorScreenplaySystemSyntax::Comment,StringToWorkWith); //Comment
				LineIter++;

				if (StringToWorkWith.StartsWith(FluidNarratorScreenplaySystemSyntax::ResponseDefault))
				{
					StringToWorkWith = StringToWorkWith.RightChop(3); // Chop Of ***
					
					ParseSelectorResponse(StringToWorkWith,TaskResponseIndexs[Task],StringToWorkWith);
					ParseSplitStringAndGetRandom(StringToWorkWith,FluidNarratorScreenplaySystemSyntax::Separator,StringToWorkWith);
					ParseCharacterID(StringToWorkWith,TaskDefaultResponseCharacterIDs[Task],StringToWorkWith,NewCharacterID);
					if (Localization)
					{
						Localization->GetLocalizationTextAsName(TaskCultures[Task],NewCharacterID,NewCharacterID);
						Localization->GetLocalizationTextAsString(TaskCultures[Task],StringToWorkWith,StringToWorkWith);
					}
					ParseVariablesApplyToString(StringToWorkWith,Variables,StringToWorkWith);
					
					if (StringToWorkWith != "")
					{
						TasksAutoResponseComponents[Task].CharacterID = NewCharacterID;
						TasksAutoResponseComponents[Task].TimerCurrent = 5;
						TasksAutoResponseComponents[Task].TimerStart = 5;
						TasksAutoResponseComponents[Task].TextAsString = StringToWorkWith;
						TasksAutoResponseComponents[Task].ResponseIndex = NewResponseIndexs.Num();
					}
				}
				
				else if (StringToWorkWith.StartsWith(FluidNarratorScreenplaySystemSyntax::Response))
				{
					StringToWorkWith = StringToWorkWith.RightChop(2); // Chop Of **
					//ParseVariablesApplyToString(StringToWorkWith,Variables,StringToWorkWith);
					ParseSelectorResponse(StringToWorkWith,TaskResponseIndexs[Task],StringToWorkWith);
					ParseSplitStringAndGetRandom(StringToWorkWith,FluidNarratorScreenplaySystemSyntax::Separator,StringToWorkWith);
					ParseCharacterID(StringToWorkWith,TaskDefaultResponseCharacterIDs[Task],StringToWorkWith,NewCharacterID);
					if (Localization)
					{
						Localization->GetLocalizationTextAsName(TaskCultures[Task],NewCharacterID,NewCharacterID);
						Localization->GetLocalizationTextAsString(TaskCultures[Task],StringToWorkWith,StringToWorkWith);
					}
					ParseVariablesApplyToString(StringToWorkWith,Variables,StringToWorkWith);
					
					NewResponsesCharacterIDs.Add(NewCharacterID);
					NewResponses.Add( FText::FromString(StringToWorkWith));
					NewResponseIndexs.Add(StringToWorkWith == ""  ? -1 : NewResponseIndexs.Num());
				}
				
				else if (StringToWorkWith.StartsWith(FluidNarratorScreenplaySystemSyntax::Text) && NewResponses.Num() == 0)
				{
					StringToWorkWith = StringToWorkWith.RightChop(1); // Chop Of *
					//ParseVariablesApplyToString(StringToWorkWith,Variables,StringToWorkWith);
					ParseSelectorResponse(StringToWorkWith,TaskResponseIndexs[Task],StringToWorkWith);
					ParseSplitStringAndGetRandom(StringToWorkWith,FluidNarratorScreenplaySystemSyntax::Separator,StringToWorkWith);
					ParseCharacterID(StringToWorkWith,TaskDefaultTextCharacterIDs[Task],StringToWorkWith,NewCharacterID);
					if (Localization)
					{
						Localization->GetLocalizationTextAsName(TaskCultures[Task],NewCharacterID,NewCharacterID);
						Localization->GetLocalizationTextAsString(TaskCultures[Task],StringToWorkWith,StringToWorkWith);
					}
					ParseVariablesApplyToString(StringToWorkWith,Variables,StringToWorkWith);
					
					if (StringToWorkWith != "")
					{
						NewTextsCharacterIDs.Add(NewCharacterID);
						NewTexts.Add(FText::FromString(StringToWorkWith));
					}
				}
				else
					break;
			}

			if (bTaskConditionsMets[Task])
			{
				bTaskIsWaitingForTextAndResponses[Task] = true;

				//Remove Responses That Did Not Pass (We Do This Here So We Can Keep Proper Response Indexes)
				for (unsigned j = NewResponseIndexs.Num() ; j-- > 0 ; )
					if (NewResponseIndexs[j] == -1)
					{
						NewResponsesCharacterIDs.RemoveAt(j);
						NewResponses.RemoveAt(j);
						NewResponseIndexs.RemoveAt(j);
					}

				const bool bHasAutoResponses = TasksAutoResponseComponents[Task].TimerStart > 0;
				
				if (TaskUserWidget)
					TaskUserWidget->OnSetTextsAndResponses(NewTextsCharacterIDs,NewResponsesCharacterIDs,NewTexts,NewResponses, NewResponseIndexs,bHasAutoResponses);
				if (TaskSlateWidget)
					TaskSlateWidget->OnSetTextsAndResponses(NewTextsCharacterIDs,NewResponsesCharacterIDs,NewTexts,NewResponses, NewResponseIndexs,bHasAutoResponses);
			}

			//No Auto Response
			else
			{
				StopAutoResponse(Task,Task);
			}

			TaskTextLinesIndexNexts[Task] += LineIter;
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task] - 2;
		}
		
		//Stop If We Finished Section
        else if (TaskLine.StartsWith(FluidNarratorScreenplaySystemSyntax::Section))
            Stop(Task,Task);
		
		//None So Go To Next Line
		else
		{
			TaskTextLinesIndexCurrents[Task] = TaskTextLinesIndexNexts[Task];
			TaskTextLinesIndexNexts[Task]++;
			bTaskConditionsMets[Task] = true;
		}

		//Stop We Reached Last Line Or Finished Section
		if (TaskTextLinesIndexNexts[Task] >= TaskLines[Task].Num())
			Stop(Task,Task);
	}
}

FString FluidNarratorScreenplaySystem::GetCurrentLineText(const int ScreenplayTask)
{
	//Return If Is Not Valid
	if (ScreenplayTask < 0 || ScreenplayTask >= FLUID_NARRATOR_SCREENPLAY_SYSTEM_TASK_MAX)
		return "";

	//No Lines
	if (TaskLines[ScreenplayTask].Num() == 0)
		return "";
	
	//Return Text Line
	FString R;
	R+= TaskLines[ScreenplayTask][TaskTextLinesIndexCurrents[ScreenplayTask]];
	return R;
}

/*=============================================================================
Fluid Narrator Screenplay System  : Parse
=============================================================================*/

void FluidNarratorScreenplaySystem::ParseDefinitionText(const FString String, FString& String_Out)
{
	//Default Outs
	String_Out = "";
	
	//Parse Input Into Lines
	TArray<FString> InputCompiledLines;
	String.ParseIntoArray(InputCompiledLines, TEXT("\n"), false);

	//Get Definition ( ~ )
	for (int i = 0; i < InputCompiledLines.Num() ; i++)
		if (InputCompiledLines[i].Contains(FluidNarratorScreenplaySystemSyntax::Definition))
		{
			InputCompiledLines[i].ReplaceInline(*FluidNarratorScreenplaySystemSyntax::Definition,TEXT(""),ESearchCase::IgnoreCase);
			ParseSplitStringAndGetFirst(InputCompiledLines[i],FluidNarratorScreenplaySystemSyntax::Comment,InputCompiledLines[i]);
			String_Out = InputCompiledLines[i];
			return;
		}
}

void FluidNarratorScreenplaySystem::ParseNamespaceText(const FString String, FString& String_Out)
{
	//Default Outs
	String_Out = "";
	
	//Parse Input Into Lines
	TArray<FString> InputCompiledLines;
	String.ParseIntoArray(InputCompiledLines, TEXT("\n"), false);

	//Get Namespace ( @ )
	for (int i = 0; i < InputCompiledLines.Num() ; i++)
		if (InputCompiledLines[i].Contains(FluidNarratorScreenplaySystemSyntax::Namespace))
		{
			InputCompiledLines[i].ReplaceInline(*FluidNarratorScreenplaySystemSyntax::Namespace,TEXT(""),ESearchCase::IgnoreCase);
			ParseSplitStringAndGetFirst(InputCompiledLines[i],FluidNarratorScreenplaySystemSyntax::Comment,InputCompiledLines[i]);
			String_Out = InputCompiledLines[i];
			return;
		}
}

void FluidNarratorScreenplaySystem::ParseGetTextsAndResponsesCombined(const FString String, TArray<FString>& Strings_Out)
{
	//Defaults Outs
	Strings_Out.Empty();

	//Parse Input Into Lines
	TArray<FString> Lines;
	String.ParseIntoArray(Lines, TEXT("\n"), false);

	//Read Lines And Look FOr Texts And Responses
	FString Line = "";
	TArray<FString> LineSplitBySelectors;
	for (int i = 0; i < Lines.Num(); i++)
	{
		//Remember Line
		Line = Lines[i];
		
		//Check If Text Or Response
		if (Line.StartsWith(FluidNarratorScreenplaySystemSyntax::Text) || Line.StartsWith(FluidNarratorScreenplaySystemSyntax::Response))
		{
			//Remove * At Beginning
			while (Line.Contains(FluidNarratorScreenplaySystemSyntax::Text) || Line.Contains(FluidNarratorScreenplaySystemSyntax::Response))
				Line.RemoveAt(0);

			//Get Text
			ParseSplitStringAndGetFirst(Line,FluidNarratorScreenplaySystemSyntax::Comment,Line);
			ParseSplitStringAndGetAll(Line,FluidNarratorScreenplaySystemSyntax::Separator, LineSplitBySelectors);
			
			//Remove [] Selector
			for (int j = 0; j < LineSplitBySelectors.Num(); j++)
				ParseSelectorResponse(LineSplitBySelectors[j],-1,LineSplitBySelectors[j]);

			//Add To String Out
			for (int j = 0; j < LineSplitBySelectors.Num(); j++)
				if (LineSplitBySelectors[j] != "" && LineSplitBySelectors[j] != ".")
					Strings_Out.Add(LineSplitBySelectors[j]);
		}
	}
}

void FluidNarratorScreenplaySystem::ParseVariablesFromString(const FString VariablesAsString, TMap<FName, FString>& VariablesMap_Out)
{
	//Defaults Outs
	VariablesMap_Out.Empty();
	
	//Parse Into Lines
	TArray<FString> VariablesAsStringLines;
	VariablesAsString.ParseIntoArray(VariablesAsStringLines, TEXT("\n"), false);
	
	//Read Variables ($)
	for (int i = 0; i < VariablesAsStringLines.Num() ; i++)
		if (VariablesAsStringLines[i].StartsWith(FluidNarratorScreenplaySystemSyntax::Variable))
		{
			//Get Line | Remove Spaces
			FString L = VariablesAsStringLines[i];
			L.TrimStartAndEndInline();
			L.ReplaceInline(TEXT(" "), TEXT(""),ESearchCase::IgnoreCase);
			
			//Do Not Want To Check Inside Sections
			if (L.Contains(FluidNarratorScreenplaySystemSyntax::Section))
				break;

			//Add If Is Valid
			int I = 0;
			L.FindChar('=',I);
			if (I != INDEX_NONE)
			{
				FName VariableName = FName(L.Left(I));
				FString VariableValue = L.Right(L.Len() - 1 - I);
				if (!VariablesMap_Out.Contains(VariableName))
					VariablesMap_Out.Add(VariableName,VariableValue);
			}
		}
}

void FluidNarratorScreenplaySystem::ParseVariablesToString(const TMap<FName,FString> VariablesMap, FString& VariablesAsString_Out)
{
	//Default Outs
	VariablesAsString_Out = "";

	//Convert Map To String
	for (const TPair<FName, FString >& Pair : VariablesMap)
		VariablesAsString_Out += Pair.Key.ToString() + "=" + Pair.Value + "\n";
}

void FluidNarratorScreenplaySystem::ParseVariablesApplyToString(const FString String, const TMap<FName, FString> Variables, FString& String_Out)
{
	//Default Outs
	String_Out = String;

	//Apply Variables
	TMap<FName, FText> exampleIntegerToActorMap;
	for (const TPair<FName, FString>& Pair : Variables)
		String_Out.ReplaceInline(*Pair.Key.ToString(), *Pair.Value,ESearchCase::IgnoreCase);
}

void FluidNarratorScreenplaySystem::ParseVariablesMetCheck(const FString String, const TMap<FName,FString> Variables, EFluidNarratorScreenplaySystemConditionCheckResults& Results_Out)
{
	//Default Outs
	Results_Out = EFluidNarratorScreenplaySystemConditionCheckResults::NoCheckNeeded;

	//Get If We Need To Check Parameters
	bool bDoCheckParameters = false;
	FString CheckString = "";
	bool bCheckStartString = false;
	{
		for (int i = 0; i < String.Len(); i++)
		{
			//Start
			if (bCheckStartString == false && String[i] == '(')
				bCheckStartString = true;

			//End
			if (bCheckStartString == true && String[i] == ')')
			{
				bCheckStartString = false;
				bDoCheckParameters = true;
			}

			//Continue
			if (bCheckStartString && String[i] != '(' && String[i] != ')')
				CheckString += String[i];
		}
	}

	//Check Parameters
	if (bDoCheckParameters)
	{
		//Start As Passed
		Results_Out = EFluidNarratorScreenplaySystemConditionCheckResults::Passed;

		//Do Checks
		TArray<FString> Checks;
		CheckString.ParseIntoArray(Checks,TEXT(","),true);
		for (int i = 0; i < Checks.Num(); i++)
		{
			if (Checks[i].Contains("="))
			{
				Checks[i].TrimStartAndEndInline();
				Checks[i].ReplaceInline(TEXT(" "), TEXT(""),ESearchCase::IgnoreCase);
				TArray<FString> CheckLeftRight;
				Checks[i].ParseIntoArray(CheckLeftRight,TEXT("="),true);
				
				if (Variables.Contains(FName(CheckLeftRight[0])) && Variables[FName(CheckLeftRight[0])] != CheckLeftRight[1])
				{
					Results_Out = EFluidNarratorScreenplaySystemConditionCheckResults::Failed;
					break;
				}
			}
		}
	}
}

void FluidNarratorScreenplaySystem::ParseVariablesSet(const FString String, TMap<FName, FString> Variables, TMap<FName, FString>& Variables_Out)
{
	//Default Outs
	Variables_Out = Variables;
	
	//Get If We Need To Apply Parameters
	bool bDoApplyParameters = false;
	FString CheckString = "";
	bool bCheckStartString = false;
	{
		for (int i = 0; i < String.Len(); i++)
		{
			//Start
			if (bCheckStartString == false && String[i] == '(')
				bCheckStartString = true;

			//End
			if (bCheckStartString == true && String[i] == ')')
			{
				bCheckStartString = false;
				bDoApplyParameters = true;
			}

			//Continue
			if (bCheckStartString && String[i] != '(' && String[i] != ')')
				CheckString += String[i];
		}
	}

	//Apply Parameters
	if (bDoApplyParameters)
	{
		//Do Applies
		TArray<FString> Checks;
		CheckString.ParseIntoArray(Checks,TEXT(","),true);
		for (int i = 0; i < Checks.Num(); i++)
		{
			if (Checks[i].Contains("="))
			{
				TArray<FString> CheckLeftRight;
				Checks[i].ParseIntoArray(CheckLeftRight,TEXT("="),true);
				CheckLeftRight[0].TrimStartAndEndInline();
				CheckLeftRight[0].ReplaceInline(TEXT(" "), TEXT(""),ESearchCase::IgnoreCase);
				CheckLeftRight[1].TrimStartAndEndInline();
				CheckLeftRight[1].ReplaceInline(TEXT(" "), TEXT(""),ESearchCase::IgnoreCase);
				
				if (Variables_Out.Contains(FName(CheckLeftRight[0])))
					Variables_Out[FName(CheckLeftRight[0])] = CheckLeftRight[1];
			}
		}
	}
}

void FluidNarratorScreenplaySystem::ParseCharacterID(const FString String, const FName CharacterIDDefault, FString& String_Out, FName& CharacterID_Out)
{
	//Default Out
	String_Out = String;
	CharacterID_Out = CharacterIDDefault;
	
	//Character ID
	if (String.Contains(":"))
	{
		int I = String.Find(":");
		CharacterID_Out = FName(String.Left(I));
		String_Out = String.Right(String.Len() - I - 1);
	}
}

void FluidNarratorScreenplaySystem::ParseSelectorResponse(const FString String, const int ResponseIndex, FString& String_Out)
{
	//Default Out
	String_Out = String;

	//Get Selector
	bool bDidStart = false;
	FString Selector;
	for (int i = 0; i < String.Len(); i++)
	{
		if (String[i] == FluidNarratorScreenplaySystemSyntax::SelectorStart.GetCharArray()[0])
		{
			bDidStart = true;
			Selector+= String[i];
		}
		else if (String[i] == FluidNarratorScreenplaySystemSyntax::SelectorEnd.GetCharArray()[0])
		{
			bDidStart = false;
			Selector+= String[i];
		}
		else if (bDidStart)
			Selector+= String[i];
	}

	//Quit As There Was No Selector
	if (Selector.Len() == 0)
		return;

	//Remove Selector From String Out
	String_Out.ReplaceInline(*Selector, TEXT(""),ESearchCase::IgnoreCase);

	//Return If Response Index Is Less Than 0 (Ignore)
	if (ResponseIndex < 0)
		return;
	
	//Chop off "[]"
	Selector.LeftChopInline(1);
	Selector.RightChopInline(1);

	//Get Random Split
	TArray<FString> SelectorSplit;
	Selector.ParseIntoArray(SelectorSplit, TEXT(","), false);

	
	
	//If Selector Is Number Its A Response Index
	for (int i = 0; i < SelectorSplit.Num(); i++)
	{
		//As Response Index
		if (SelectorSplit[i].IsNumeric())
		{
			int AsInt = FCString::Atoi(*SelectorSplit[i]);
			if (ResponseIndex != AsInt)
				String_Out = "";
		}
	}
}

void FluidNarratorScreenplaySystem::ParseSplitStringAndGetRandom(const FString String, const FString SplitBy, FString& String_Out)
{
	//Default Out
	String_Out = "";
	
	//Can't Split So Just Return Input
	if (!String.Contains(SplitBy))
	{
		String_Out = String;
		return;;
	}

	//Get Random Split
	TArray<FString> StringSplit;
	String.ParseIntoArray(StringSplit,*SplitBy, false);
	String_Out = StringSplit[FMath::RandRange(0,StringSplit.Num() - 1)];
	String_Out.TrimStartAndEndInline();
}

void FluidNarratorScreenplaySystem::ParseSplitStringAndGetAll(const FString String, const FString SplitBy, TArray<FString>& Strings_Out)
{
	//Default Out
	Strings_Out.Empty();

	//Return If Not Valid Input
	if (String == "" || SplitBy == "")
		return;
	
	//Get Out
	if (!String.Contains(SplitBy))
		Strings_Out.Add(String);
	else String.ParseIntoArray(Strings_Out, *SplitBy, false);

	//Trim
	for (int i = 0; i < Strings_Out.Num(); i++)
		Strings_Out[i].TrimStartAndEndInline();
}

void FluidNarratorScreenplaySystem::ParseSplitStringAndGetFirst(const FString String, const FString SplitBy, FString& String_Out)
{
	//Default Out
	String_Out = "";

	//Return If Not Valid Input
	if (String == "" || SplitBy == "")
		return;
	
	//Can't Split So Just Return Input
	if (!String.Contains(SplitBy))
	{
		String_Out = String;
		return;
	}
	
	//Get Random Split
	TArray<FString> StringSplit;
	String.ParseIntoArray(StringSplit, *SplitBy, false);
	String_Out = StringSplit[0];
	String_Out.TrimStartAndEndInline();
}

void FluidNarratorScreenplaySystem::ParseSplitStringUpToSpace(const FString String, FString& String_Out)
{
	//Default Out
	String_Out = "";

	//Get Up To First Space
	for (int i = 0; i < String.Len(); i++)
		if (String[i] != ' ')
			String_Out += String[i];
		else break;
}

