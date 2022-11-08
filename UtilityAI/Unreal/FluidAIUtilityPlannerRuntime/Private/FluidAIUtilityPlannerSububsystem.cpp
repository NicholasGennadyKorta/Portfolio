// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidAIUtilityPlannerSubsystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Developer Settings

 UFluidAIUtilityPlannerDeveloperSettings::UFluidAIUtilityPlannerDeveloperSettings()
{
	DebugInput.AddKey = EKeys::Add;
	DebugInput.SubtractKey = EKeys::Subtract;
 	DebugInput.ClearKey = EKeys::NumPadZero;
 	DebugTask.TextColor = FColor::White;
 	DebugTask.TextScale = 1;
 	DebugTask.bTextHasShadow = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Virtual Blackboard

void UFluidAIUtilityPlannerVirtualBlackboard::SetVector(const FName Key, const FVector Value)
{
	//Set Vector
	if (Vectors.Contains(Key))
		Vectors[Key] = Value;
	else
		Vectors.Add(Key,Value);
}

FVector UFluidAIUtilityPlannerVirtualBlackboard::GetVector(const FName Key)
{
	//Get Vector
	if (Vectors.Contains(Key))
		return Vectors[Key];
	return FVector::Zero();
}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveVector(const FName Key)
{
	//Remove Vector
	if (Vectors.Contains(Key))
		Vectors.Remove(Key);
}

void UFluidAIUtilityPlannerVirtualBlackboard::SetRotator(const FName Key, const FRotator Value)
{
	//Set Rotator
	if (Rotators.Contains(Key))
		Rotators[Key] = Value;
	else
		Rotators.Add(Key,Value);
}

FRotator UFluidAIUtilityPlannerVirtualBlackboard::GetRotator(const FName Key)
{
	//Get Rotator
	if (Rotators.Contains(Key))
		return Rotators[Key];
	return FRotator::ZeroRotator;
}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveRotator(const FName Key)
{
	//Remove Rotator
	if (Rotators.Contains(Key))
		Rotators.Remove(Key);
}

void UFluidAIUtilityPlannerVirtualBlackboard::SetInt(const FName Key, const int Value)
{
	//Set Int
	if (Ints.Contains(Key))
		Ints[Key] = Value;
	else
		Ints.Add(Key,Value);
}

int UFluidAIUtilityPlannerVirtualBlackboard::GetInt(const FName Key)
{
	//Get Ints
	if (Ints.Contains(Key))
		return Ints[Key];
	return 0;
}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveInt(const FName Key)
{
	//Remove Int
	if (Ints.Contains(Key))
		Ints.Remove(Key);
}

void UFluidAIUtilityPlannerVirtualBlackboard::SetByte(const FName Key, const uint8 Value)
{
	//Set Byte
	if (Bytes.Contains(Key))
		Bytes[Key] = Value;
	else
		Bytes.Add(Key,Value);
}

uint8 UFluidAIUtilityPlannerVirtualBlackboard::GetByte(const FName Key)
{
	//Get Byte
	if (Bytes.Contains(Key))
		return Bytes[Key];
	return 0;

}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveByte(const FName Key)
{
	//Remove Byte
	if (Bytes.Contains(Key))
		Bytes.Remove(Key);
}

void UFluidAIUtilityPlannerVirtualBlackboard::SetFloat(const FName Key, const float Value)
{
	//Set Float
	if (Floats.Contains(Key))
		Floats[Key] = Value;
	else
		Floats.Add(Key,Value);
}

float UFluidAIUtilityPlannerVirtualBlackboard::GetFloat(const FName Key)
{
	//Get Float
	if (Floats.Contains(Key))
		return Floats[Key];
	return 0;
}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveFloat(const FName Key)
{
	//Remove Float
	if (Floats.Contains(Key))
		Floats.Remove(Key);
}

void UFluidAIUtilityPlannerVirtualBlackboard::SetDouble(const FName Key, const double Value)
{
	//Set Double
	if (Doubles.Contains(Key))
		Doubles[Key] = Value;
	else
		Doubles.Add(Key,Value);
}

double UFluidAIUtilityPlannerVirtualBlackboard::GetDouble(const FName Key)
{
	//Get Double
	if (Doubles.Contains(Key))
		return Doubles[Key];
	return 0;
}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveDouble(const FName Key)
{
	//Remove Double
	if (Doubles.Contains(Key))
		Doubles.Remove(Key);
}

void UFluidAIUtilityPlannerVirtualBlackboard::SetName(const FName Key, const FName Value)
{
 	//Set Name
 	if (Names.Contains(Key))
 		Names[Key] = Value;
 	else
 		Names.Add(Key,Value);
}

FName UFluidAIUtilityPlannerVirtualBlackboard::GetName(const FName Key)
{
 	//Get Name
 	if (Names.Contains(Key))
 		return Names[Key];
 	return "";
}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveName(const FName Key)
{
	 //Remove Name
 	if (Names.Contains(Key))
 		Names.Remove(Key);
}

void UFluidAIUtilityPlannerVirtualBlackboard::SetString(const FName Key, const FString Value)
{
 	//Set String
 	if (Strings.Contains(Key))
 		Strings[Key] = Value;
 	else
 		Strings.Add(Key,Value);
}

FString UFluidAIUtilityPlannerVirtualBlackboard::GetString(const FName Key)
{
 	//Get String
 	if (Strings.Contains(Key))
 		return Strings[Key];
 	return "";
}

void UFluidAIUtilityPlannerVirtualBlackboard::RemoveString(const FName Key)
{
 	//Remove String
 	if (Strings.Contains(Key))
 		Strings.Remove(Key);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Event Board

void UFluidAIUtilityPlannerEventBoard::ExecuteEventStart_Implementation()
{
	
}

void UFluidAIUtilityPlannerEventBoard::ExecuteEventEnd_Implementation()
{
	
}

void UFluidAIUtilityPlannerEventBoard::ExecuteEventPreUpdate_Implementation()
{
	
}

void UFluidAIUtilityPlannerEventBoard::ExecuteEventCustom_Implementation(FName Event)
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Agent Blueprint Action

void UFluidAIUtilityPlannerAgentBlueprintAction::OnInitialized_Implementation()
 {
 }


void UFluidAIUtilityPlannerAgentBlueprintAction::ExecuteAgentActionStart_Implementation()
{
}

void UFluidAIUtilityPlannerAgentBlueprintAction::ExecuteAgentActionRestart_Implementation()
{
	
}

void UFluidAIUtilityPlannerAgentBlueprintAction::ExecuteAgentActionEnd_Implementation()
{
}

void UFluidAIUtilityPlannerAgentBlueprintAction::ExecuteAgentActionTick_Implementation()
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Subsystem : Main

void UFluidAIUtilityPlannerSubsystem::Initialize(FSubsystemCollectionBase & Collection)
{
	//Return If Already Initialized | Set Initialized
	if (bInitialized) return;
	InitializeAgents();
	bInitialized = true;

	DomainModelAsyncUtilityEnabled = false;
	DomainModelAsyncPlannerEnabled = false;
}

void UFluidAIUtilityPlannerSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;

	//Agent Update
	AgentUpdateStart(DeltaTime);
	AgentUpdateEventBoardPreUpdate(DeltaTime);
	AgentUpdateUtility(DeltaTime);
 	AgentUpdateTransformBindings(DeltaTime);
	AgentUpdateEnd(DeltaTime);

	//Tasks AI Controllers
	AgentBlueprintActions(DeltaTime);

	//Debug
	DebugTaskNameAll();
	DebugTaskNameSelected();
}

bool UFluidAIUtilityPlannerSubsystem::IsTickable() const
{
	//Validate
	return !IsTemplate() && bInitialized;
}

bool UFluidAIUtilityPlannerSubsystem::IsTickableInEditor() const
{
	return true;
}

bool UFluidAIUtilityPlannerSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidAIUtilityPlannerSubsystem::GetStatId() const
{
	return TStatId();
}

UWorld* UFluidAIUtilityPlannerSubsystem::HelperGetWorld() const
{
 	return GetWorld();	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Subsystem : Agent Create Destroy

void UFluidAIUtilityPlannerSubsystem::InitializeAgents()
{
	// Initialize Agent Available Queue With All Possible Entities
	for (int i = 0; i < FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; ++i)
		AgentsAvailable.push(i);
}

void UFluidAIUtilityPlannerSubsystem::CreateAgent(UFluidAIUtilityPlannerAgent* AgentAsset, const FName UtilitySubModelName, int& Agent_Out)
{
	//Invalid Agent By Default
	Agent_Out = -1;
	
	//Return If We Reached Max Agent Count Or Agent Is Not Valid
	if (AgentCount >=FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || AgentAsset == nullptr)
		return;

	//Get Utility Sub Model Index
	int SubModelIndex = -1;
	 {
		 for (int i = 0; i < AgentAsset->AgentUtilitySubModels.Num(); i++)
		 	if (AgentAsset->AgentUtilitySubModels[i].Name == UtilitySubModelName)
		 	{
		 		SubModelIndex = i;
		 		break;
		 	}
		
		//Return If Valid Sub Model
		if (SubModelIndex == - 1)
			return;
	 }
	
	//Create Agent
	Agent_Out = AgentsAvailable.front();
	AgentsAvailable.pop();
	AgentActives[Agent_Out] = true;
	AgentJustCreateds[Agent_Out] = true;
	AgentSetToDeletes[Agent_Out] = false;
	++AgentCount;

 	//Create Agent GUID
 	AgentGuidAsNames[Agent_Out] = FName(FGuid::NewGuid().ToString());
 	
	//Set Agent Update Interval Data
	AgentUpdateIntervals[Agent_Out] = AgentAsset->Settings.UpdateInterval;
	AgentUpdateIntervalSteps[Agent_Out] = FMath::FRandRange(0, AgentAsset->Settings.UpdateInterval);

	//Set Agent Utility Data
	AgentUtilitySubModels[Agent_Out] = &AgentAsset->AgentUtilitySubModels[SubModelIndex];
	AgentUtilityOutputNames[Agent_Out] = "";

 	//Set Transform
 	AgentTransformActorBindings[Agent_Out] = nullptr;
 	AgentTransformLocations[Agent_Out] = FVector::ZeroVector;

	//Event Board
 	const auto EventBoard = AgentAsset->Settings.EventBoard;
	if (EventBoard != nullptr)
	{
		const auto EventBoardObject = EventBoard.GetDefaultObject();
		EventBoardObject->FluidAIUtilityPlannerSubsystem = this;
		AgentEventBoards[Agent_Out] = EventBoardObject;
	}
	
	//Tasks
	AgentActionNamePreviouses[Agent_Out] = "";
	AgentActionNameCurrents[Agent_Out] = "";
 	AgentActionIndexPreviouses[Agent_Out] = -1;
 	AgentActionIndexCurrents[Agent_Out] = -1;
 	AgentActionModes[Agent_Out] = (uint8)AgentAsset->Settings.AgentActionMode;
	
	//Virtual Blackboards
	AgentVirtualBlackboards[Agent_Out] = NewObject<UFluidAIUtilityPlannerVirtualBlackboard>();
}

void UFluidAIUtilityPlannerSubsystem::DestroyAgent(const int Agent)
{
	//Return If Agent Is Not Valid
	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX)
		return;

 	AgentActives[Agent] = false;
	AgentSetToDeletes[Agent] = true;

 	//Get Event Board End (WE CALL IT HERE AS WE WANT TO IMMEDIATELY CALL END SO WE DON"T HAVE NULL VARIABLES)
 	const auto EventBoard = AgentEventBoards[Agent];
 	if (EventBoard != nullptr)
 	{
 		EventBoard->Agent = Agent;
 		EventBoard->ExecuteEventEnd();
 	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Subsystem :  Asset Utilities

void UFluidAIUtilityPlannerSubsystem::GetAssetsUtilitySubModelNamesWithTag(UFluidAIUtilityPlannerAgent* AgentAsset, const FName Tag, TArray<FName>& UtilitySubModelNames_Out, int& UtilitySubModelNamesNum_Out, bool& bUtilitySubModelWithTagFound)
{
 	//Return If Agent Asset Is Not Valid
	if (AgentAsset == nullptr)
		return;
 	
 	//Get Models With Tag
 	UtilitySubModelNames_Out.Empty();
 	for (int i = 0; i < AgentAsset->AgentUtilitySubModels.Num(); i++)
 		if (AgentAsset->AgentUtilitySubModels[i].Tags.Contains(Tag))
 			UtilitySubModelNames_Out.AddUnique(AgentAsset->AgentUtilitySubModels[i].Name);

 	//Return
 	UtilitySubModelNamesNum_Out = UtilitySubModelNames_Out.Num();
 	bUtilitySubModelWithTagFound = UtilitySubModelNamesNum_Out > 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Subsystem : Agent Data

void UFluidAIUtilityPlannerSubsystem::SetAgentToForceUpdate(const int Agent, const bool bClearOutputs)
{
 	//Return If Is Not Valid
 	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;
 	
	//Set Update Interval Step To 0 So Agent Gets Updated Next Frame
	AgentUpdateIntervalSteps[Agent] = 0;

	//Clear Outputs
	if (bClearOutputs)
	{
		AgentUtilityOutputNames[Agent] = "";

		AgentActionStates[Agent] = -1;
		AgentActionNameCurrents[Agent] = "";
		AgentActionNamePreviouses[Agent] = "";
		AgentActionIndexPreviouses[Agent] = -1;
		AgentActionIndexCurrents[Agent] = -1;
	}
}

void UFluidAIUtilityPlannerSubsystem::GetAgentGuidAsFName(const int Agent, FName& GuidAsFName_Out)
{
 	//Return If Is Not Valid
 	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 	{
 		GuidAsFName_Out = "";
 		return;
 	}

 	GuidAsFName_Out = AgentGuidAsNames[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentUtilitySubModelName(const int Agent, FName& Name_Out)
{
	//Return If Is Not Valid 
	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
	{
		Name_Out = "";
		return;
	}

	//Get Agent Utility Sub Model Name
	Name_Out = AgentUtilitySubModels[Agent]->Name;
}

void UFluidAIUtilityPlannerSubsystem::SetAgentUtilityInputValueByName(const int Agent, const FName Name, const uint8& Value)
 {
 	//Return If Is Not Valid
 	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	//Set Agent Utility Input
 	for (int i = 0; i < AgentUtilitySubModels[Agent]->InputNames.Num(); i++)
 		if (AgentUtilitySubModels[Agent]->InputNames[i] == Name)
 		{
 			AgentUtilityInputs[Agent][i] = Value;
 			return;
 		}
 }

void UFluidAIUtilityPlannerSubsystem::GetAgentUtilityInputByName(const int Agent, const FName Name, uint8& Value_Out)
{
	//Return If Agent Is Not Valid
	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
		return;

 	//Get Agent Utility Input
	for (int i = 0; i < AgentUtilitySubModels[Agent]->InputNames.Num(); i++)
		if (AgentUtilitySubModels[Agent]->InputNames[i] == Name)
			Value_Out = AgentUtilityInputs[Agent][i];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentUtilityInputsByName(const int Agent, TArray<FName>& Names_Out, TArray<uint8>& Values_Out)
{
	//Force Out Values To Be Zeroed Out
	Names_Out.Empty();
	Values_Out.Empty();
	
	//Return If Agent Is Not Valid
	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
		return;

	//Get Names And Inputs
	for (int i = 0; i <	AgentUtilitySubModels[Agent]->InputNames.Num(); i++)
	{
		Names_Out.Add(AgentUtilitySubModels[Agent]->InputNames[i]);
		Values_Out.Add(AgentUtilityInputs[Agent][i]);
	}
}

void UFluidAIUtilityPlannerSubsystem::GetAgentUtilityOutputName(const int Agent, FName& Value_Out)
{
	//Force Out Values To Be Zeroed Out
	Value_Out = "";
	
	//Return If Agent Is Not Valid
	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
		return;

	//Get Agent Utility Output
	Value_Out =	AgentUtilityOutputNames[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentTaskNameCurrent(const int Agent, FName& Value_Out)
{
	//Force Out Values To Be Zeroed Out
	Value_Out = "";
	
	//Return If Agent Is Not Valid
	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
		return;

	//Get Agent Task Name Currents
	Value_Out =	AgentActionNameCurrents[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentTaskNamePrevious(const int Agent, FName& Value_Out)
{
	//Force Out Values To Be Zeroed Out
	Value_Out = "";
	
	//Return If Agent Is Not Valid
	if (Agent < 0 ||Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
		return;

	//Get Agent Task Name Previous
	Value_Out =	AgentActionNamePreviouses[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentTransformActorBinding(const int Agent, AActor*& Actor_Out)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		Actor_Out = nullptr;

 	Actor_Out = AgentTransformActorBindings[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentTransformActorBindingUnsafe(const int Agent, AActor*& Actor_Out)
{
 	Actor_Out = AgentTransformActorBindings[Agent];
}

void UFluidAIUtilityPlannerSubsystem::SetAgentTransformActorBinding(const int Agent, AActor* Actor)
{
	 //Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	//Set Actor Binding
 	AgentTransformActorBindings[Agent] = Actor;
}

void UFluidAIUtilityPlannerSubsystem::GetAgentTransformLocation(const int Agent, FVector& Location_Out)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;
 	
 	Location_Out = AgentTransformLocations[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentTransformLocationUnsafe(const int Agent,FVector& Location_Out)
{
 	Location_Out = AgentTransformLocations[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentVirtualBlackboard(const int Agent, UFluidAIUtilityPlannerVirtualBlackboard*& VirtualBlackboard_Out)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;
 	
	VirtualBlackboard_Out = AgentVirtualBlackboards[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentVirtualBlackboardUnsafe(const int Agent, UFluidAIUtilityPlannerVirtualBlackboard*& VirtualBlackboard_Out)
{
 	VirtualBlackboard_Out = AgentVirtualBlackboards[Agent];
}

void UFluidAIUtilityPlannerSubsystem::SetAgentActor(const int Agent, AActor* Actor)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	AgentActors[Agent] = Actor;
}

void UFluidAIUtilityPlannerSubsystem::GetAgentActor(const int Agent, AActor*& Actor_Out)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	Actor_Out = AgentActors[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentActorUnsafe(const int Agent, AActor*& Actor_Out)
{
 	Actor_Out = AgentActors[Agent];
}

void UFluidAIUtilityPlannerSubsystem::SetAgentPawn(const int Agent,  APawn* Pawn)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	AgentPawns[Agent] = Pawn;
}

void UFluidAIUtilityPlannerSubsystem::GetAgentPawn(const int Agent, APawn*& Pawn_Out)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	Pawn_Out = AgentPawns[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentPawnUnsafe(const int Agent, APawn*& Pawn_Out)
{
 	Pawn_Out = AgentPawns[Agent];
}

void UFluidAIUtilityPlannerSubsystem::SetAgentSkeletalMeshComponent(const int Agent, USkeletalMeshComponent* SkeletalMeshComponent)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	AgentSkeletalMeshComponents[Agent] = SkeletalMeshComponent;
}

void UFluidAIUtilityPlannerSubsystem::GetAgentSkeletalMeshComponent(const int Agent, USkeletalMeshComponent*& SkeletalMeshComponent_Out)
{
 	//Return If Agent Is Not Valid
 	if (Agent < 0 || Agent >= FLUID_AI_UTILITY_PLANNER_AGENTS_MAX || !AgentActives[Agent])
 		return;

 	SkeletalMeshComponent_Out = AgentSkeletalMeshComponents[Agent];
}

void UFluidAIUtilityPlannerSubsystem::GetAgentSkeletalMeshComponentUnsafe(const int Agent, USkeletalMeshComponent*& SkeletalMeshComponent_Out)
{
 	SkeletalMeshComponent_Out = AgentSkeletalMeshComponents[Agent];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Subsystem : Utility Output

void UFluidAIUtilityPlannerSubsystem::AgentUpdateStart(const float DeltaTime)
{
	//Get Stats For This Code (Total)
 	SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Start);
		{
			for (int i = 0; i <  FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
				if (AgentActives[i])
					AgentUpdateIntervalSteps[i] -= DeltaTime;
		}
	}
}

void UFluidAIUtilityPlannerSubsystem::AgentUpdateEnd(const float DeltaTime)
{
	//Get Stats For This Code (Total)
 	SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_End);
		{
			for (int i = 0; i <  FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
			{
				//Agent Update Interval Step Reset
				AgentJustCreateds[i] = false;
				if (AgentActives[i] && AgentUpdateIntervalSteps[i] < 0)
					AgentUpdateIntervalSteps[i] = AgentUpdateIntervals[i];

				//Delete Agent
				if (AgentSetToDeletes[i])
				{
					// Put Destroyed Agent Back In Available Que (At The Back) | Decrease Agent Count
					AgentsAvailable.push(i);
					AgentSetToDeletes[i] = false;
					--AgentCount;

					//Disable Actions
					AgentActionModes[i] = false;

					//Remove Guid
					AgentGuidAsNames[i] = "";

					//Clear Transform Actor Binding | Agent Pawn | Agent Actor
					AgentTransformActorBindings[i] = nullptr;
					AgentPawns[i] = nullptr;
					AgentActors[i] = nullptr;
				}
			}
		}
	}
}

void UFluidAIUtilityPlannerSubsystem::AgentUpdateEventBoardPreUpdate(const float DeltaTime)
{
	//Get Stats For This Code (Total)
 	SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Event_Board);
		{
			for (int i = 0; i <  FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
			{
				//Continue If Agent Is Not Active Or Interval Steps Not 0
				if (!AgentActives[i] || AgentUpdateIntervalSteps[i] > 0 && !AgentJustCreateds[i])
					continue;

				//Get Event Board
				const auto EventBoard = AgentEventBoards[i];
				
				//Event Board Begin | PreUpdate
				if (EventBoard != nullptr)
				{
					//Set Agent
					EventBoard->Agent = i;
					
					//PreUpdate
					if (!AgentJustCreateds[i])
						EventBoard->ExecuteEventPreUpdate();
					
					//Begin
					else
						EventBoard->ExecuteEventStart();
				}
			}
		}
	}
}


void UFluidAIUtilityPlannerSubsystem::AgentUpdateUtility(const float DeltaTime)
{
	//Get Stats For This Code (Total)
 	SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Utility);
		{
			//Cache Agent Data For Better Performance
			FFluidAIUtilityPlannerAgentUtilitySubModel* AgentUtilitySubModel;
			TArray<FRichCurve> UtilityCurves;
			FName UtilityOutputName;
			float AgentUtilityNew = 0;
			float AgentUtilityBest = 0;
			int AgentUtilityBestIndex = 0;

			for (int i = 0; i <  FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
			{
				//Continue If Agent Is Not Active Or Interval Steps Not 0
				if (!AgentActives[i] || AgentUpdateIntervalSteps[i] > 0 && !AgentJustCreateds[i])
					continue;

				//Do Utility Input
				{
					AgentUtilityNew = 0;
					AgentUtilityBest = 0;
					AgentUtilitySubModel = AgentUtilitySubModels[i];
					UtilityCurves = AgentUtilitySubModel->Curves;
		
					//Get Best Utility Input
					for (uint8  j = 0; j < UtilityCurves.Num() && j < FLUID_AI_UTILITY_PLANNER_UTILITY_INPUTS_MAX; j++)
					{
						AgentUtilityNew = UtilityCurves[j].Eval(AgentUtilityInputs[i][j] * 0.01);
				
						if (j == 0 || AgentUtilityNew > AgentUtilityBest)
						{
							AgentUtilityBest = AgentUtilityNew;
							AgentUtilityBestIndex = j;
						}
					}
					
					//Get Utility Output Name
					UtilityOutputName = AgentUtilityOutputNames[i] = AgentUtilitySubModel->InputNames[AgentUtilityBestIndex];
					if (AgentUtilitySubModel->OutputNameModifiers.Contains(UtilityOutputName))
						UtilityOutputName = AgentUtilityOutputNames[i] = AgentUtilitySubModel->OutputNameModifiers[UtilityOutputName];
					

					//Set Action Current | Set Action State To Begin
					if (AgentActionModes[i])
					{
						AgentActionNamePreviouses[i] = AgentActionNameCurrents[i];
						AgentActionNameCurrents[i] = UtilityOutputName;
						AgentActionIndexPreviouses[i] = AgentActionIndexCurrents[i];
						AgentActionIndexCurrents[i] = AgentActionToIndexMap.Contains(UtilityOutputName) ? AgentActionToIndexMap[UtilityOutputName] : -1;
						AgentActionStates[i] = 1;
					}
				}
			}
		}
	}
}

void UFluidAIUtilityPlannerSubsystem::AgentUpdateTransformBindings(const float DeltaTime)
 {
	 //Get Stats For This Code (Total)
 	SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Total);
	 {
 		//Get Stats For This Code
 		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Transform_Bindings);
 		{
 			for (int i = 0; i <  FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
 			{
 				//Actor
 				if (!AgentTransformActorBindings[i])
 					continue;
 				
 				auto const Transform = AgentTransformActorBindings[i]->GetTransform();
 				AgentTransformLocations[i] = Transform.GetLocation();
 			}
 		}
	 }
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner Subsystem : Agent Blueprint Actions

void UFluidAIUtilityPlannerSubsystem::AgentBlueprintActions(float DeltaTime)
{
	//Get Stats For This Code (Total)
	SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Update_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Agent_Actions_Blueprint_Actions);
		{
			for (int i = 0; i < FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
			{
				//Only Not Deleting | AI Controller Is Valid
				if (AgentActionModes[i] != 1) // Agent Blueprint Action
					continue;
				
				const uint8 AgentActionState = AgentActionStates[i];
				const int AgentActionIndexCurrent = AgentActionIndexCurrents[i];
				const int AgentActionIndexPrevious = AgentActionIndexPreviouses[i];
			
				//Tick
				if (AgentActionState == 2 && AgentActionIndexCurrent != -1)
				{
					const auto Agent = AgentActionArray[AgentActionIndexCurrent];
					Agent->Agent = i;
					Agent->ExecuteAgentActionTick();
				}
			
				//End Start
				else if (AgentActionState == 1 && AgentActionIndexCurrent != AgentActionIndexPrevious)
				{
					//End
					if (AgentActionIndexPrevious != -1)
					{
						const auto Action = AgentActionArray[AgentActionIndexPrevious];
						Action->Agent = i;
						Action->ExecuteAgentActionEnd();
					}

					//Start
					if (AgentActionIndexCurrent != -1)
					{
						const auto Action = AgentActionArray[AgentActionIndexCurrent];
						Action->Agent = i;
						Action->ExecuteAgentActionStart();
					}

					//Set Task State To Tick
					AgentActionStates[i] = 2;
				}

				//Restart
				else if (AgentActionState == 1)
				{
					//Restart
					if (AgentActionIndexCurrent != -1)
					{
						const auto Action = AgentActionArray[AgentActionIndexCurrent];
						Action->Agent = i;
						Action->ExecuteAgentActionRestart();
					}

					//Set Task State To Tick
					AgentActionStates[i] = 2;
				}
			}
		}
	}
}

void UFluidAIUtilityPlannerSubsystem::AddBlueprintAgentActions(TMap<FName, TSubclassOf<UFluidAIUtilityPlannerAgentBlueprintAction>> Actions)
{
	//Add (Will Overwrite)
	for (const TPair<FName, TSubclassOf<UFluidAIUtilityPlannerAgentBlueprintAction>>& Action : Actions)
	{
		//Make Sure Task Is Valid
		if (Action.Value == nullptr)
			continue;

		//Cache Default Object
		auto ActionDefaultObject = Action.Value.GetDefaultObject();
		
		//Remove If Already Present By Key Name
		if (AgentActionToIndexMap.Contains(Action.Key))
		{
			AgentActionToIndexMap.Remove(Action.Key);
			AgentActionArray.Remove(ActionDefaultObject);
		}

		//Add | Initialize
		ActionDefaultObject->FluidAIUtilityPlannerSubsystem = this;
		AgentActionToIndexMap.Add(Action.Key,AgentActionArray.Num());
		AgentActionArray.Add(ActionDefaultObject);
		ActionDefaultObject->OnInitialized();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Utility Planner : Debug

static TAutoConsoleVariable<float> CDebugFluidAIUtilityPlannerDebug(
	TEXT("Fluid.AI.UtilityPlanner.Debug"),
	0,
	TEXT("Fluid.AI.UtilityPlanner.Debug\n"),
	ECVF_SetByConsole);

void UFluidAIUtilityPlannerSubsystem::DebugTaskNameAll() const
{
	//1 == All
	if ( CDebugFluidAIUtilityPlannerDebug.GetValueOnAnyThread() == 1)
	{
		//Get Stats For This Code (Total)
		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Debug_Total);
		{
			//Get Stats For This Code
			SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Debug_Task_Name);
			{
				//If Debug On
				if (CDebugFluidAIUtilityPlannerDebug.GetValueOnAnyThread() == 1)
				{
					TArray<int> Agents;
					for (int i = 0; i < FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
						if (AgentActives[i])
							Agents.Add(i);
					DebugTaskName(Agents);
				}
			}
		}
	}
}

void UFluidAIUtilityPlannerSubsystem::DebugTaskNameSelected()
{
	//2 == Selected Debug Actors
	if (CDebugFluidAIUtilityPlannerDebug.GetValueOnAnyThread() == 2)
	{
		//Get Stats For This Code (Total)
		SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Debug_Total);
		{
			//Get Stats For This Code
			SCOPE_CYCLE_COUNTER(STAT_FluidAIUtilityPlanner_Debug_Task_Name);
			{
				//Cache World
				UWorld* World = GetWorld();
				
				//Get Developer Settings Debug
				const auto DeveloperSettingsDebugInput = GetMutableDefault<UFluidAIUtilityPlannerDeveloperSettings>()->DebugInput;
				
				//Get First Player Controller
				const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
				if (PlayerController)
				{
					//Get Pawn
					const APawn* Pawn = PlayerController->GetPawnOrSpectator();
					if (Pawn)
					{
						bool bAdd = PlayerController->WasInputKeyJustPressed(DeveloperSettingsDebugInput.AddKey);
						bool bSubtract = PlayerController->WasInputKeyJustPressed(DeveloperSettingsDebugInput.SubtractKey);
						bool bClear = PlayerController->WasInputKeyJustPressed(DeveloperSettingsDebugInput.ClearKey);

						//Add | Subtract
						if (bAdd || bSubtract)
						{
							//Source
							const FVector SourceLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
							const FVector SourceForward = PlayerController->PlayerCameraManager->GetCameraRotation().Quaternion().GetForwardVector();
							const double SourceDistance = 5000000;
							
							//Get Closest Sensor Tracker
							int ClosestSensor = -1;
							float ClosestDot = 0.75f;
							for (int i = 0; i < FLUID_AI_UTILITY_PLANNER_AGENTS_MAX; i++)
							{
								if (AgentActives[i])
								{
									const double Distance =  FVector::Distance(SourceLocation,AgentTransformLocations[i]);
									FVector Direction = (AgentTransformLocations[i] - SourceLocation).GetSafeNormal();
									float Dot = FVector::DotProduct(SourceForward,Direction);
								
									if (Dot > ClosestDot && Distance <= SourceDistance)
									{
										ClosestSensor = i;
										ClosestDot = Dot;
									}
								}
							}

							//Add | Subtract
							if (bAdd)
								DebugAgents.AddUnique(ClosestSensor);
							if (bSubtract)
								DebugAgents.Remove(ClosestSensor);
						}

						//Clear
						if (bClear)
							DebugAgents.Empty();
					}
				}

				//Remove Inactive Debug Agents
				for (int i = DebugAgents.Num(); i --> 0;)	
					if (!AgentActives[DebugAgents[i]])
						DebugAgents.RemoveAt(i);
				
				//Debug
				DebugTaskName(DebugAgents);
			}
		}
	}
}

void UFluidAIUtilityPlannerSubsystem::DebugTaskName(const TArray<int> Agents) const
{
	//Get Developer Settings Debug
	const auto DeveloperSettingsDebugTask = GetMutableDefault<UFluidAIUtilityPlannerDeveloperSettings>()->DebugTask;
	
	//Cache World
	const UWorld* World = GetWorld();

	//Each Sensor
	for (int i = 0; i < Agents.Num(); i++)
	{
		//Get Agent
		const int Agent = Agents[i];
		if (AgentActives[Agent])
			DrawDebugString(World,AgentTransformLocations[Agent],AgentActionNameCurrents[Agent].ToString(),0,DeveloperSettingsDebugTask.TextColor,0,DeveloperSettingsDebugTask.bTextHasShadow,DeveloperSettingsDebugTask.TextScale);
	}
}