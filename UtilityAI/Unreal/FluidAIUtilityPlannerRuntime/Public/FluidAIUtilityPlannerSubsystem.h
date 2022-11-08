// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <queue>
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "FluidAIUtilityPlannerSubsystem.generated.h"

//Fluid Entities AI Utility Planner Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid AI Utility Planner"), STATGROUP_FluidAIUtilityPlanner, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Start"),STAT_FluidAIUtilityPlanner_Update_Start,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update End"),STAT_FluidAIUtilityPlanner_Update_End,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Event Board"),STAT_FluidAIUtilityPlanner_Update_Event_Board,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Utility"),STAT_FluidAIUtilityPlanner_Update_Utility,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Agent Actions [Blueprint Action]"),STAT_FluidAIUtilityPlanner_Agent_Actions_Blueprint_Actions,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Agent Actions [Custom00]"),STAT_FluidAIUtilityPlanner_Agent_Actions_Custom00,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Agent Actions [Custom01]"),STAT_FluidAIUtilityPlanner_Agent_Actions_Custom01,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Agent Actions [Custom02]"),STAT_FluidAIUtilityPlanner_Agent_Actions_Custom02,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Transform Bindings"),STAT_FluidAIUtilityPlanner_Update_Transform_Bindings,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Update Total"),STAT_FluidAIUtilityPlanner_Update_Total,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Debug Total"),STAT_FluidAIUtilityPlanner_Debug_Total,STATGROUP_FluidAIUtilityPlanner);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Utility Planner : Debug Task Name"),STAT_FluidAIUtilityPlanner_Debug_Task_Name,STATGROUP_FluidAIUtilityPlanner);

//Fluid Entities AI Utility Strips Global Constants
const int FLUID_AI_UTILITY_PLANNER_AGENTS_MAX = 10000;
const int FLUID_AI_UTILITY_PLANNER_UTILITY_INPUTS_MAX = 25;

class UFluidAIUtilityPlannerEventBoard;
class UFluidAIUtilityPlannerAgentBlueprintAction;
class UFluidAIUtilityPlannerSubsystem;

//Fluid AI Utility Planner Developer Settings Debug Profile
USTRUCT()
struct FLUIDAIUTILITYPLANNERRUNTIME_API FFluidAIUtilityPlannerDeveloperSettingsDebug
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		FColor TextColor;
	UPROPERTY(EditDefaultsOnly)
		float TextScale;
	UPROPERTY(EditDefaultsOnly)
		bool bTextHasShadow;
};

//Fluid AI Utility Planner Developer Settings Debug Profile
USTRUCT()
struct FLUIDAIUTILITYPLANNERRUNTIME_API FFluidAIUtilityPlannerDeveloperSettingsDebugInput
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		FKey AddKey;
	UPROPERTY(EditDefaultsOnly)
		FKey SubtractKey;
	UPROPERTY(EditDefaultsOnly)
		FKey ClearKey;
};

//Fluid AI Utility Planner Developer Settings
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Fluid AI Utility Planner"))
class FLUIDAIUTILITYPLANNERRUNTIME_API UFluidAIUtilityPlannerDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UFluidAIUtilityPlannerDeveloperSettings();
	
	UPROPERTY(Config,EditDefaultsOnly)
		FFluidAIUtilityPlannerDeveloperSettingsDebug DebugTask;
	UPROPERTY(Config,EditDefaultsOnly)
		FFluidAIUtilityPlannerDeveloperSettingsDebugInput DebugInput;
};

//Fluid AI Utility Planner Virtual Blackboard
UCLASS()
class FLUIDAIUTILITYPLANNERRUNTIME_API UFluidAIUtilityPlannerVirtualBlackboard : public UObject
{
	GENERATED_BODY()

public:

	//Vector
	UFUNCTION(BlueprintCallable)
		void SetVector(const FName Key, const FVector Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FVector GetVector(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveVector(const FName Key);
	UPROPERTY()
		TMap<FName,FVector> Vectors;

	//Vector
	UFUNCTION(BlueprintCallable)
		void SetRotator(const FName Key, const FRotator Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FRotator GetRotator(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveRotator(const FName Key);
	UPROPERTY()
		TMap<FName,FRotator> Rotators;

	//Ints
	UFUNCTION(BlueprintCallable)
		void SetInt(const FName Key, const int Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		int GetInt(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveInt(const FName Key);
	UPROPERTY()
		TMap<FName,int> Ints;

	//Bytes
	UFUNCTION(BlueprintCallable)
		void SetByte(const FName Key, const uint8 Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		uint8 GetByte(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveByte(const FName Key);
	UPROPERTY()
		TMap<FName,uint8> Bytes;

	//Floats
	UFUNCTION(BlueprintCallable)
		void SetFloat(const FName Key, const float Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		float GetFloat(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveFloat(const FName Key);
	UPROPERTY()
		TMap<FName,uint8> Floats;

	//Doubles
	UFUNCTION(BlueprintCallable)
		void SetDouble(const FName Key, const double Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		double GetDouble(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveDouble(const FName Key);
	UPROPERTY()
		TMap<FName,uint8> Doubles;

	//Name
	UFUNCTION(BlueprintCallable)
		void SetName(const FName Key, const FName Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FName GetName(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveName(const FName Key);
	UPROPERTY()
		TMap<FName,FName> Names;

	//String
	UFUNCTION(BlueprintCallable)
		void SetString(const FName Key, const FString Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FString GetString(const FName Key);
	UFUNCTION(BlueprintCallable)
		void RemoveString(const FName Key);
	UPROPERTY()
	TMap<FName,FString> Strings;
};

UENUM(Blueprintable)
enum class EFFluidAIUtilityPlannerAgentActionMode: uint8
{
	None						UMETA(DisplayName = "None"),
	AgentBlueprintAction		UMETA(DisplayName = "Agent Blueprint Action"),
	Custom0						UMETA(DisplayName = "Custom0"),
	Custom1						UMETA(DisplayName = "Custom1"),
	Custom2						UMETA(DisplayName = "Custom2"),
};

//Fluid Entities AI Utility Planner Agent Utility SubModel
USTRUCT()
struct FLUIDAIUTILITYPLANNERRUNTIME_API FFluidAIUtilityPlannerAgentUtilitySubModel
{
	GENERATED_BODY()
	
	UPROPERTY()
		FName Name;
	UPROPERTY()
		TArray<FRichCurve> Curves;
	UPROPERTY()
		TArray<FName> InputNames;
	UPROPERTY()
		TMap<FName,FName> OutputNameModifiers;
	UPROPERTY()
		TArray<FName> Tags;
	UPROPERTY()
		FVector2D NodePosition;
};

//Fluid Entities AI Utility Planner Agent Settings
USTRUCT()
struct FLUIDAIUTILITYPLANNERRUNTIME_API FFluidAIUtilityPlannerAgentSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UFluidAIUtilityPlannerEventBoard> EventBoard;
	UPROPERTY(EditDefaultsOnly)
		float UpdateInterval;
	UPROPERTY(EditDefaultsOnly)
		EFFluidAIUtilityPlannerAgentActionMode AgentActionMode;
	UPROPERTY()
		FVector2D NodePosition;
};
	
//Fluid Entities AI Utility Planner Agent
UCLASS(BlueprintType)
class FLUIDAIUTILITYPLANNERRUNTIME_API UFluidAIUtilityPlannerAgent : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
		TArray<FName> AgentUtilityModelInputNames;
	UPROPERTY()
		TArray<FFluidAIUtilityPlannerAgentUtilitySubModel> AgentUtilitySubModels;
	UPROPERTY()
		FFluidAIUtilityPlannerAgentSettings Settings;
	UPROPERTY()
		FVector2D AgentUtilityModelNodePosition;
};

//Fluid AI Utility Planner Event Board
UCLASS(BlueprintType, Blueprintable)
class FLUIDAIUTILITYPLANNERRUNTIME_API UFluidAIUtilityPlannerEventBoard : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
		void ExecuteEventStart();
	UFUNCTION(BlueprintNativeEvent)
		void ExecuteEventEnd();
	UFUNCTION(BlueprintNativeEvent)
		void ExecuteEventPreUpdate();
	UFUNCTION(BlueprintNativeEvent)
		void ExecuteEventCustom(FName Event);

	virtual void ExecuteEventStart_Implementation();
	virtual void ExecuteEventEnd_Implementation();
	virtual void ExecuteEventPreUpdate_Implementation();
	virtual void ExecuteEventCustom_Implementation(FName Event);

	UPROPERTY(BlueprintReadOnly)
		UFluidAIUtilityPlannerSubsystem* FluidAIUtilityPlannerSubsystem;
	UPROPERTY(BlueprintReadOnly)
		int Agent;	
};

//Fluid AI Utility Planner Blueprint Action
UCLASS(BlueprintType, Blueprintable)
class FLUIDAIUTILITYPLANNERRUNTIME_API UFluidAIUtilityPlannerAgentBlueprintAction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
		void OnInitialized();
	UFUNCTION(BlueprintNativeEvent)
		void ExecuteAgentActionStart();
	UFUNCTION(BlueprintNativeEvent)
		void ExecuteAgentActionRestart();
	UFUNCTION(BlueprintNativeEvent)
		void ExecuteAgentActionEnd();
	UFUNCTION(BlueprintNativeEvent)
		void ExecuteAgentActionTick();

	virtual void OnInitialized_Implementation();
	virtual void ExecuteAgentActionStart_Implementation();
	virtual void ExecuteAgentActionRestart_Implementation();
	virtual void ExecuteAgentActionEnd_Implementation();
	virtual void ExecuteAgentActionTick_Implementation();
	
	UPROPERTY(BlueprintReadOnly)
		UFluidAIUtilityPlannerSubsystem* FluidAIUtilityPlannerSubsystem;
	UPROPERTY(BlueprintReadOnly)
		int Agent;
};

//Fluid AI Utility Planner Subsystem
UCLASS(BlueprintType)
class FLUIDAIUTILITYPLANNERRUNTIME_API UFluidAIUtilityPlannerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid AI Utility Planner Subsystem : Main
public:
	
	void Initialize(FSubsystemCollectionBase & Collection) override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable)
		UWorld* HelperGetWorld() const;

	bool bInitialized;

//Fluid AI Utility Planner Subsystem : Domain Model
public:
	
	bool DomainModelAsyncUtilityEnabled;
	bool DomainModelAsyncPlannerEnabled;
	
//Fluid AI Utility Planner Subsystem : Main
public:

	UFUNCTION(BlueprintInternalUseOnly)
		void InitializeAgents();
	UFUNCTION(BlueprintCallable)
		void CreateAgent(UFluidAIUtilityPlannerAgent* AgentAsset, const FName UtilitySubModelName, int& Agent_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyAgent(const int Agent);
	
	std::queue<int> AgentsAvailable;
	int AgentCount;

//Fluid AI Utility Planner Subsystem : Asset Utilities
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
		void GetAssetsUtilitySubModelNamesWithTag(UFluidAIUtilityPlannerAgent* AgentAsset, const FName Tag, TArray<FName>& UtilitySubModelNames_Out, int& UtilitySubModelNamesNum_Out, bool& bUtilitySubModelWithTagFound);
	
//Fluid AI Utility Planner Subsystem : Agent Data
public:
	
	UFUNCTION(BlueprintCallable)
		void SetAgentToForceUpdate(const int Agent, const bool bClearOutputs);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentGuidAsFName(const int Agent, FName& GuidAsFName_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentUtilitySubModelName(const int Agent, FName& Name_Out);
	UFUNCTION(BlueprintCallable)
		void SetAgentUtilityInputValueByName(const int Agent, const FName Name, const uint8& Value);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentUtilityInputByName(const int Agent, const FName Name, uint8& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentUtilityInputsByName(const int Agent, TArray<FName>& Names_Out, TArray<uint8>& Values_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentUtilityOutputName(const int Agent, FName& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentTaskNameCurrent(const int Agent, FName& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentTaskNamePrevious(const int Agent, FName& Value_Out);
	UFUNCTION(BlueprintCallable)
		void SetAgentTransformActorBinding(const int Agent, AActor* Actor);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentTransformActorBinding(const int Agent, AActor*& Actor_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentTransformActorBindingUnsafe(const int Agent, AActor*& Actor_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentTransformLocation(const int Agent, FVector& Location_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentTransformLocationUnsafe(const int Agent, FVector& Location_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentVirtualBlackboard(const int Agent, UFluidAIUtilityPlannerVirtualBlackboard*& VirtualBlackboard_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentVirtualBlackboardUnsafe(const int Agent, UFluidAIUtilityPlannerVirtualBlackboard*& VirtualBlackboard_Out);
	UFUNCTION(BlueprintCallable)
		void SetAgentActor(const int Agent, AActor* Actor);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentActor(const int Agent, AActor*& Actor_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentActorUnsafe(const int Agent, AActor*& Actor_Out);
	UFUNCTION(BlueprintCallable)
		void SetAgentPawn(const int Agent, APawn* Pawn);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentPawn(const int Agent, APawn*& Pawn_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentPawnUnsafe(const int Agent, APawn*& Pawn_Out);
	UFUNCTION(BlueprintCallable)
		void SetAgentSkeletalMeshComponent(const int Agent, USkeletalMeshComponent* SkeletalMeshComponent);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentSkeletalMeshComponent(const int Agent, USkeletalMeshComponent*& SkeletalMeshComponent_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetAgentSkeletalMeshComponentUnsafe(const int Agent, USkeletalMeshComponent*& SkeletalMeshComponent_Out);
	

	//GUID
	FName AgentGuidAsNames[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	
	//Update | Active
	float AgentUpdateIntervals[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	float AgentUpdateIntervalSteps[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	bool AgentJustCreateds[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	bool AgentActives[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	bool AgentSetToDeletes[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	
	//Event Boards
	UFluidAIUtilityPlannerEventBoard* AgentEventBoards[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	
	//Utility
	FFluidAIUtilityPlannerAgentUtilitySubModel* AgentUtilitySubModels[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	uint8 AgentUtilityInputs[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX][FLUID_AI_UTILITY_PLANNER_UTILITY_INPUTS_MAX];
	FName AgentUtilityOutputNames[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];

	//Action
	FName AgentActionNameCurrents[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	FName AgentActionNamePreviouses[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	int AgentActionIndexCurrents[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	int AgentActionIndexPreviouses[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	uint8 AgentActionStates[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	uint8 AgentActionModes[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];

	//Transform
	AActor* AgentTransformActorBindings[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	FVector AgentTransformLocations[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];

	//Actor | Pawn
	AActor* AgentActors[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	APawn* AgentPawns[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];
	USkeletalMeshComponent* AgentSkeletalMeshComponents[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];

	//Other
	UFluidAIUtilityPlannerVirtualBlackboard* AgentVirtualBlackboards[FLUID_AI_UTILITY_PLANNER_AGENTS_MAX];

//Fluid AI Utility Planner Subsystem : Agent Update
public:

	void AgentUpdateStart(const float DeltaTime);
	void AgentUpdateEnd(const float DeltaTime);
	void AgentUpdateEventBoardPreUpdate(const float DeltaTime);
	void AgentUpdateUtility(const float DeltaTime);
	void AgentUpdateTransformBindings(const float DeltaTime);

//Fluid AI Utility Planner Subsystem : Agent Actions
public:
	
	void AgentBlueprintActions(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void AddBlueprintAgentActions(TMap<FName,TSubclassOf<UFluidAIUtilityPlannerAgentBlueprintAction>> AgentActions);
	
	TMap<FName,int> AgentActionToIndexMap;
	TArray<UFluidAIUtilityPlannerAgentBlueprintAction*> AgentActionArray;
	

//Fluid AI Utility Planner Subsystem : Debug
public:
	
	void DebugTaskNameAll() const;
	void DebugTaskNameSelected();
	void DebugTaskName(const TArray<int> Agents) const;

	TArray<int> DebugAgents;
};