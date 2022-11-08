// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidEntitiesCharacterHybridSubsystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Debugging

static TAutoConsoleVariable<int32> CDebugFluidEntitiesCharacterHybridPlayerInput(
	TEXT("Fluid.Entities.Character.Hybrid.Input.Player.Debug"),
	0,
	TEXT("Fluid.Entities.Character.Hybrid.Input.Player.Debug\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);

static TAutoConsoleVariable<int32> CDebugFluidEntitiesCharacterHybridPlayerTransform(
	TEXT("Fluid.Entities.Character.Hybrid.Transform.Player.Debug"),
	0,
	TEXT("Fluid.Entities.Character.Hybrid.Transform.Player.Debug\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);


static TAutoConsoleVariable<int32> CDebugFluidEntitiesCharacterHybridPlayerMovement(
	TEXT("Fluid.Entities.Character.Hybrid.Movement.Player.Debug"),
	0,
	TEXT("Fluid.Entities.Character.Hybrid.Movement.Player.Debug\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);

static TAutoConsoleVariable<int32> CDebugFluidEntitiesCharacterHybridOrientationWarping(
	TEXT("Fluid.Entities.Character.Hybrid.Orientation.Warping.Debug"),
	0,
	TEXT("Fluid.Entities.Character.Hybrid.Orientation.Warping.Debug\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);

static TAutoConsoleVariable<int32> CDebugFluidEntitiesCharacterHybridDistanceMatching(
	TEXT("Fluid.Entities.Character.Hybrid.Distance.Matching.Debug"),
	0,
	TEXT("Fluid.Entities.Character.Hybrid.Distance.Matching.Debug\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Character Main

AFluidEntitiesCharacterHybridCharacter::AFluidEntitiesCharacterHybridCharacter()
{
	//No Tick By Default
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void AFluidEntitiesCharacterHybridCharacter::BeginPlay()
{
	//Parent
	Super::BeginPlay();
	
	//Get Subsystem
	FluidEntitiesCharacterHybridSubsystem = GetGameInstance()->GetSubsystem<UFluidEntitiesCharacterHybridSubsystem>();
	if (!FluidEntitiesCharacterHybridSubsystem)
		return;

	//Create Entity (Destroy Actor If Failed)
	FluidEntitiesCharacterHybridSubsystem->CreateEntity(this,this,FluidEntitiesCharacterHybridEntity);
	if (FluidEntitiesCharacterHybridEntity == -1)
		Destroy(true);
	else HelperEventEntityCreated();
}

void AFluidEntitiesCharacterHybridCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Parent
	Super::EndPlay(EndPlayReason);

	//Do Not Do On Quit Or Will Crash
	if (EndPlayReason == EEndPlayReason::Quit)
		return;

	//Destroy Entity
	if (FluidEntitiesCharacterHybridSubsystem)
		FluidEntitiesCharacterHybridSubsystem->DestroyEntity(FluidEntitiesCharacterHybridEntity);
}

void AFluidEntitiesCharacterHybridCharacter::PossessedBy(AController* NewController)
{
	//Parent
	Super::PossessedBy(NewController);

	//Delayed By Frame So BeginPlay Is Finished
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda([=]()
	{
		//Set Local Player Index
		if (NewController->IsLocalPlayerController())
		{
			FluidEntitiesCharacterHybridSubsystem->SetEntityInputStateComponentLocalPlayerIndex(FluidEntitiesCharacterHybridEntity,0);
			HelperEventPossessedByPlayerLocal((APlayerController*)NewController,0);
		}
		else FluidEntitiesCharacterHybridSubsystem->SetEntityInputStateComponentLocalPlayerIndex(FluidEntitiesCharacterHybridEntity,-1);
	}), 0.001f, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Character Helper

void AFluidEntitiesCharacterHybridCharacter::HelperMoveInput(const float X, const float Y)
{
	//Set Entity Input
	FluidEntitiesCharacterHybridSubsystem->SetEntityInputComponentInputUnsafe(FluidEntitiesCharacterHybridEntity,X,Y);

	//Get Entity Movement Component
	FFluidEntitiesCharacterHybridMovementComponent MovementComponent;
	FluidEntitiesCharacterHybridSubsystem->GetEntityMovementComponentUnsafe(FluidEntitiesCharacterHybridEntity,MovementComponent);

	//Add Movement Input
	AddMovementInput(MovementComponent.Direction,MovementComponent.Gait);
}

void AFluidEntitiesCharacterHybridCharacter::HelperMoveDirection(FVector2D Direction, const float Gait)
{
	//Normalize With Gait | Move
	Direction = Direction.GetSafeNormal();

	//Set Entity Input
	FluidEntitiesCharacterHybridSubsystem->SetEntityInputComponentInputUnsafe(FluidEntitiesCharacterHybridEntity,Direction.X,Direction.Y);

	//Get Entity Movement Component
	FFluidEntitiesCharacterHybridMovementComponent MovementComponent;
	FluidEntitiesCharacterHybridSubsystem->GetEntityMovementComponentUnsafe(FluidEntitiesCharacterHybridEntity,MovementComponent);
	MovementComponent.Gait = Gait;

	//Add Movement Input
	AddMovementInput(MovementComponent.Direction,MovementComponent.Gait);
	
}

void AFluidEntitiesCharacterHybridCharacter::HelperMoveToLocation(FVector Location, const float Gait)
{
	//Convert Point To Direction And Move
	Location = (Location - GetActorLocation()).GetSafeNormal();
	HelperMoveDirection(FVector2d(Location.X,Location.Y),Gait);
}

void AFluidEntitiesCharacterHybridCharacter::HelperToggleEntityMovementComponentStrafe()
{
	//Get Entity Movement Component
	FFluidEntitiesCharacterHybridMovementComponent MovementComponent;
	FluidEntitiesCharacterHybridSubsystem->GetEntityMovementComponentUnsafe(FluidEntitiesCharacterHybridEntity,MovementComponent);

	//Toggle Strafe
	MovementComponent.bStrafe = !MovementComponent.bStrafe;

	//Set Entity Movement Component
	FluidEntitiesCharacterHybridSubsystem->SetEntityMovementComponentUnsafe(FluidEntitiesCharacterHybridEntity,MovementComponent);
}

void AFluidEntitiesCharacterHybridCharacter::HelperSetActorMovementJumpProperties(const float GravityScale, const float BrakingDecelerationFalling, const float AirControl, const float AirControlBoostMultiplier, const float AirControlBoostVelocityThreshold, const float FallingLateralFriction, const float JumpMaxTime, const float JumpZVelocity)
{
	const auto ActorCharacterMovementComponent = GetCharacterMovement();
	ActorCharacterMovementComponent->GravityScale = GravityScale;
	ActorCharacterMovementComponent->BrakingDecelerationFalling = BrakingDecelerationFalling;
	ActorCharacterMovementComponent->AirControl = AirControl;
	ActorCharacterMovementComponent->AirControlBoostMultiplier = AirControlBoostMultiplier;
	ActorCharacterMovementComponent->AirControlBoostVelocityThreshold = AirControlBoostVelocityThreshold;
	ActorCharacterMovementComponent->FallingLateralFriction = FallingLateralFriction;
	ActorCharacterMovementComponent->JumpZVelocity = JumpZVelocity;
	JumpMaxHoldTime = JumpMaxTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Subsystem Main

void UFluidEntitiesCharacterHybridSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//Return If Already Initialized | Set Initialized
	if (bInitialized) return;
	bInitialized = true;

	//Get Core Subsystem | Entities Manager
	const auto CoreSubsystem = GetGameInstance()->GetSubsystem<UFluidEntitiesCoreSubsystem>();
	EntitiesManager = CoreSubsystem->EntitiesManager;

	//Register Entities Components
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FFluidEntitiesCharacterHybridTransformComponent,"CharacterHybridTransform");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FFluidEntitiesCharacterHybridInputXYComponent,"CharacterHybridInputXY");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FFluidEntitiesCharacterHybridInputYawComponent,"CharacterHybridInputYaw");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FFluidEntitiesCharacterHybridInputStateComponent,"CharacterHybridInputState");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FFluidEntitiesCharacterHybridDistanceMatchingComponent,"CharacterHybridDistanceMatching");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FFluidEntitiesCharacterHybridOrientationWarpingComponent,"CharacterHybridOrientationWarping");
	FLUID_ENTITIES_CORE_COMPONENT_REGISTER(EntitiesManager,FFluidEntitiesCharacterHybridMovementComponent,"CharacterHybridMovement");
	
	//Get Pointers To Components
	TransformComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FFluidEntitiesCharacterHybridTransformComponent,"CharacterHybridTransform");
	InputXYComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FFluidEntitiesCharacterHybridInputXYComponent,"CharacterHybridInputXY");
	InputYawComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FFluidEntitiesCharacterHybridInputYawComponent,"CharacterHybridInputYaw");
	InputStateComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FFluidEntitiesCharacterHybridInputStateComponent,"CharacterHybridInputState");
	DistanceMatchingComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FFluidEntitiesCharacterHybridDistanceMatchingComponent,"CharacterHybridDistanceMatching");
	OrientationWarpingComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FFluidEntitiesCharacterHybridOrientationWarpingComponent,"CharacterHybridOrientationWarping");
	MovementComponents = FLUID_ENTITIES_CORE_COMPONENTS(EntitiesManager,FFluidEntitiesCharacterHybridMovementComponent,"CharacterHybridMovement");

	//Init Entities Character System States
	EntitiesCharacterSystemStates.Init(true,8);
}

void UFluidEntitiesCharacterHybridSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;

	EntitiesCharacterActorBindingsSystem(DeltaTime);
	EntitiesCharacterCharacterBindingsSystem(DeltaTime);
	EntitiesCharacterInputSystem(DeltaTime);
	EntitiesCharacterDistanceMatchingSystem(DeltaTime);
	EntitiesCharacterOrientationWarpingSystem(DeltaTime);
}

bool UFluidEntitiesCharacterHybridSubsystem::IsTickable() const
{
	return !IsTemplate() && bInitialized;
}

bool UFluidEntitiesCharacterHybridSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidEntitiesCharacterHybridSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidEntitiesCharacterHybridSubsystem::GetStatId() const
{
	return TStatId();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Create Destroy

void UFluidEntitiesCharacterHybridSubsystem::CreateEntity(AActor* ActorToBindTo, AFluidEntitiesCharacterHybridCharacter* FluidEntitiesCharacterHybridCharacterToBindTo, int& Entity_Out)
{
	//Get Core Subsystem | Entities Manager
	const auto CoreSubsystem = GetGameInstance()->GetSubsystem<UFluidEntitiesCoreSubsystem>();
	EntitiesManager = CoreSubsystem->EntitiesManager;

	//Create Entity
	CoreSubsystem->CreateEntity(Entity_Out);

	//Return If Entity Not Valid
	if (Entity_Out < 0 || Entity_Out >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;
	
	//Set Entity To Character Hybrid Signature
	SetEntityToEntityCharacterHybridSignature(Entity_Out,true);
	
	//Set Not Player Index By Default (If Entity Was Previously Used)
	SetEntityInputStateComponentLocalPlayerIndex(Entity_Out,-1);
	
	//Set Entity Actor Binding
	if (ActorToBindTo != nullptr && !EntityActorBindingToEntitys.Contains(ActorToBindTo))
	{
		EntityActorBindings[Entity_Out] = ActorToBindTo;
		EntityActorBindingToEntitys.Add(ActorToBindTo,Entity_Out);
	}

	//Set Entity Character Binding
	if (FluidEntitiesCharacterHybridCharacterToBindTo != nullptr && !EntityCharacterBindingToEntitys.Contains(FluidEntitiesCharacterHybridCharacterToBindTo))
	{
		EntityCharacterBindings[Entity_Out] = FluidEntitiesCharacterHybridCharacterToBindTo;
		EntityCharacterBindingToEntitys.Add(FluidEntitiesCharacterHybridCharacterToBindTo,Entity_Out);
	}
}

void UFluidEntitiesCharacterHybridSubsystem::DestroyEntity(const int Entity)
{
	//Return If Entity Not Valid
	if (Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;
	
	//Get Core Subsystem | Entities Manager
	const auto CoreSubsystem = GetGameInstance()->GetSubsystem<UFluidEntitiesCoreSubsystem>();
	EntitiesManager = CoreSubsystem->EntitiesManager;

	//Unset Entity To Character Signature
	SetEntityToEntityCharacterHybridSignature(Entity,false);
	
	//Unset Entity Actor Binding
	if (EntityActorBindingToEntitys.Contains(EntityActorBindings[Entity]))
	{
		EntityActorBindingToEntitys.Remove(EntityActorBindings[Entity]);
		EntityActorBindings[Entity] = nullptr;
	}

	//Unset Entity Character Binding
	if (EntityCharacterBindingToEntitys.Contains(EntityCharacterBindings[Entity]))
	{
		EntityCharacterBindingToEntitys.Remove(EntityCharacterBindings[Entity]);
		EntityCharacterBindings[Entity] = nullptr;
	}
	
	//Destroy Entity
	CoreSubsystem->DestroyEntity(Entity);
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityActorBindingEntity(AActor* Actor, int& Entity_Out)
{
	//Default Out
	Entity_Out = -1;

	//Get Entity Actor Binding Entity
	if (EntityActorBindingToEntitys.Contains(Actor))
		Entity_Out = EntityActorBindingToEntitys[Actor];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityCharacterBindingEntity(AFluidEntitiesCharacterHybridCharacter* FluidEntitiesCharacterHybridCharacter, int& Entity_Out)
{
	//Default Out
	Entity_Out = -1;

	//Get Entity Character Binding Entity
	if (EntityCharacterBindingToEntitys.Contains(FluidEntitiesCharacterHybridCharacter))
		Entity_Out = EntityCharacterBindingToEntitys[FluidEntitiesCharacterHybridCharacter];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Systems

void UFluidEntitiesCharacterHybridSubsystem::EntitiesCharacterActorBindingsSystem(const float DeltaTime)
{
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidEntitiesCharacter_ActorBinding);
	{
		//Cache Repeated Variables (Better Performance)
		AActor* EntityActorBinding;
		
		for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		{
			//No Actor Binding So Continue
			EntityActorBinding = EntityActorBindings[i];
			if (EntityActorBinding == nullptr)
				continue;
			
			//Write Entity Transform Component
			const auto TransformComponent = &TransformComponents[i];
			TransformComponent->Location = EntityActorBinding->GetActorLocation();
			TransformComponent->ForwardVector = EntityActorBinding->GetActorForwardVector();
			TransformComponent->RightVector = EntityActorBinding->GetActorRightVector();
			TransformComponent->Rotation = EntityActorBinding->GetActorRotation();

			//Write Entity Movement Component
			const auto MovementComponent = &MovementComponents[i];
			MovementComponent->Velocity = EntityActorBinding->GetVelocity();
		}
	}
}

void UFluidEntitiesCharacterHybridSubsystem::EntitiesCharacterCharacterBindingsSystem(const float DeltaTime)
{
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidEntitiesCharacter_CharacterBinding);
	{
		//Cache Repeated Variables (Better Performance)
		ACharacter* EntityCharacterBinding;
		
		for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		{
			//No Actor Binding So Continue
			EntityCharacterBinding = EntityCharacterBindings[i];
			if (EntityCharacterBinding == nullptr)
				continue;
			
			//Write Entity Transform Component
			const auto TransformComponent = &TransformComponents[i];
			TransformComponent->Location = EntityCharacterBinding->GetActorLocation();
			TransformComponent->ForwardVector = EntityCharacterBinding->GetActorForwardVector();
			TransformComponent->RightVector = EntityCharacterBinding->GetActorRightVector();
			TransformComponent->Rotation = EntityCharacterBinding->GetActorRotation();

			//Write Entity Movement Component
			const auto MovementComponent = &MovementComponents[i];
			MovementComponent->bIsFalling = EntityCharacterBinding->GetMovementComponent()->IsFalling();
			MovementComponent->Velocity = EntityCharacterBinding->GetVelocity();
		}
	}
}

void UFluidEntitiesCharacterHybridSubsystem::EntitiesCharacterInputSystem(const float DeltaTime)
{
	//Return If State Is Disabled
	if (!EntitiesCharacterSystemStates[0])
		return;
	
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidEntitiesCharacter_Input);
	{
		//Cache Repeated Variables (Better Performance)
		FRotator TransformRotation = FRotator::ZeroRotator;
		int LocalPlayerIndex = 0;
		int Entity = 0;
		float YawInput = 0;
		float Yaw = 0;
		float YawDelta = 0;
		float X = 0;
		float Y = 0;
		float XY = 0;
		float XYPrevious = 0;
		FVector Velocity = FVector::Zero();
		float VelocitySize = 0;
		float MovementLeanLeftRight = 0;
		bool bJustStartedInput = false;
		bool bJustChangedInput = false;
		
		for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		{
			//Read Data
			Entity = i;

			//Not Character Hybrid Entity So Skip
			if (!CharacterHybridEntitiesSignatures[Entity])
				continue;
			
			const auto TransformComponent = &TransformComponents[Entity];
			const auto InputXYComponent = &InputXYComponents[Entity];
			const auto InputYawComponent = &InputYawComponents[Entity];
			const auto InputStateComponent = &InputStateComponents[Entity];
			const auto MovementCompnent = &MovementComponents[Entity];

			//Input Is Disabled So Zero It Out
			if (InputStateComponent->bDisabled)
			{
				InputXYComponents[Entity] = FFluidEntitiesCharacterHybridInputXYComponent();
				InputYawComponents[Entity] = FFluidEntitiesCharacterHybridInputYawComponent();
				InputStateComponent->bDoesHaveInput = false;
			}
			
			//Update Input If Not Disabled
			else
			{
				//Input XY
				XYPrevious = InputXYComponent->XYPrevious = InputXYComponent->XY;
				X = InputXYComponent->X;
				Y = InputXYComponent->Y;
				XY = InputXYComponent->XY = FMath::Min(FVector2D(X,Y).Size(), 1.0f);

				//Get Local Player Index (Clamped)
				LocalPlayerIndex = InputStateComponent->LocalPlayerIndex;
				LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, -1, 7);
				InputStateComponent->LocalPlayerIndex = LocalPlayerIndex;
				
				//Input State (Just Changed Input)
				bJustChangedInput = InputStateComponent->bJustChangedInput = XY != XYPrevious || LocalPlayerIndex >= 0;
            	
				//Input State (Za Reset) | Input Yaw
				if (bJustChangedInput)
				{
					//Get Transform Rotation | Controller Rotation
					TransformRotation = TransformComponent->Rotation;
					
					//Za Rest Of Input State 
					InputStateComponent->bDoesHaveInput = XY != 0;
					InputStateComponent->bJustStoppedInput = XYPrevious != 0 && XY == 0;
					bJustStartedInput = InputStateComponent->bJustStartedInput = XYPrevious == 0 && XY != 0;
					
					//Yaw Input
					YawInput = FMath::RadiansToDegrees(FMath::Atan2(X,Y));

					//Yaw Delta | Yaw (Default)
					Yaw = 0;
					YawDelta = 0;
            		
					//Yaw Delta | Yaw (Player Controller)
					if (LocalPlayerIndex >= 0)
					{
						//Get Player Controller
						const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);

						//Set Yaw To Camera Yaw
						Yaw = PlayerController->PlayerCameraManager->GetCameraRotation().Yaw;
						YawDelta = UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleDelta(Yaw,TransformRotation.Yaw);
					}
    
					//Get Final Yaw | Final Delta Yaw
					Yaw = InputYawComponent->Yaw = YawInput + Yaw;
					YawDelta = InputYawComponent->YawDelta = UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleDelta(YawDelta,YawInput);

					//Start Yaw Delta | Yaw
					if (bJustStartedInput)
					{
						InputYawComponent->YawDeltaStart = YawDelta;
						InputYawComponent->YawStart = Yaw;
					}

					//Yaw Dot
					//InputYawComponent->YawDot = FVector::DotProduct(TransformRotation.Vector(), FRotator(0, YawInput, 0).Vector());
				}
			}

			//Movement Component
			{
				//Get Velocity
				Velocity = MovementCompnent->Velocity;
				VelocitySize = MovementCompnent->VelocitySize = Velocity.Size2D();
				MovementCompnent->VelocityRate = VelocitySize / MovementCompnent->VelocityMaxWanted;
				
				//Get Movement | Movement Rate
				if (LocalPlayerIndex >= 0)
					MovementCompnent->Direction = UKismetMathLibrary::CreateVectorFromYawPitch(Yaw,0,XY);
				else
					MovementCompnent->Direction = FVector(X,Y,0);
				
				//Get Lean Left Right
				MovementLeanLeftRight = FVector::DotProduct(TransformComponent->RightVector, Velocity);
				MovementCompnent->LeanLeftRight = UKismetMathLibrary::Lerp(MovementCompnent->LeanLeftRight, (FMath::Abs(MovementLeanLeftRight) / 360) * FMath::Sign(MovementLeanLeftRight) ,0.2f);
			}
			
			//Player Debug Input
			if (LocalPlayerIndex >= 0)
			{
				//Input
				if (CDebugFluidEntitiesCharacterHybridPlayerInput.GetValueOnAnyThread() != 0)
				{
					//OnScreen Debug Messages
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,   InputStateComponent->bDisabled == true ? "Input Locked : True" : "Input Locked : False");
					//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Yaw Dot : " + FString::SanitizeFloat(InputYawComponent->YawDot));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Yaw Start : " + FString::SanitizeFloat(InputYawComponent->YawStart));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Yaw : " + FString::SanitizeFloat(InputYawComponent->Yaw));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Delta Yaw Start : " + FString::SanitizeFloat(InputYawComponent->YawDeltaStart));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Delta Yaw : " + FString::SanitizeFloat(InputYawComponent->YawDelta));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "XY : " + FString::SanitizeFloat(InputXYComponent->XY));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "Y : " + FString::SanitizeFloat(InputXYComponent->Y));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, "X : " + FString::SanitizeFloat(InputXYComponent->X));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "Fluid Entities Character Hybrid Player 0 : Input Components");
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "");
				}

				//Transform
				if (CDebugFluidEntitiesCharacterHybridPlayerTransform.GetValueOnAnyThread() != 0)
				{
					//OnScreen Debug Messages
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow,"Right : " + TransformComponent->RightVector.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow,"Forward : " + TransformComponent->ForwardVector.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow,"Location : " + TransformComponent->Location.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor(177,181,249),"Rotation : " + TransformComponent->Rotation.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "Fluid Entities Character Hybrid Player 0 : Transform Component");
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "");
				}
			
				//Player Debug Movement (Might As Well Do It Here)
				if (CDebugFluidEntitiesCharacterHybridPlayerMovement.GetValueOnAnyThread() != 0)
				{
					//OnScreen Debug Messages
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,"Strafe : " +  FString(MovementCompnent->bStrafe ? "True" : "False"));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,"Is Falling : " +  FString(MovementCompnent->bIsFalling ? "True" : "False"));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Lean LeftRight : " +  FString::SanitizeFloat(MovementCompnent->LeanLeftRight));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Gait : " +  FString::SanitizeFloat(MovementCompnent->Gait));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Velocity Max Wanted : " +  FString::SanitizeFloat(MovementCompnent->VelocityMaxWanted));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Velocity Rate : "+  FString::SanitizeFloat(MovementCompnent->VelocityRate));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Velocity Size : "+  FString::SanitizeFloat(MovementCompnent->VelocitySize));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow,"Velocity : " + MovementCompnent->Velocity.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow,"Movement : " + MovementCompnent->Direction.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "Fluid Entities Character Hybrid Player 0 : Movement Component");
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "");
				}
			}
		}
	}
}

void UFluidEntitiesCharacterHybridSubsystem::EntitiesCharacterDistanceMatchingSystem(const float DeltaTime)
{
	//Return If State Is Disabled
	if (!EntitiesCharacterSystemStates[1])
		return;
	
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidEntitiesCharacter_DistanceMatching);
	{
		//Cache Repeated Variables (Better Performance)
		int Entity = 0;
		float SmallVelocity = 0;
		FVector CurrentVelocityInFrame = FVector::ZeroVector;
		FVector CurrentVelocityDirection = FVector::ZeroVector;
		FVector DeaccelerationInFrame = FVector::ZeroVector;
		int StopFrameCountToZeroVelocity = 0;
		float StoppingDistance = 0.0f;
		
		for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		{
			//Read Data
			Entity = i;

			//Not Character Hybrid Entity So Skip
			if (!CharacterHybridEntitiesSignatures[Entity])
				continue;
			
			const auto DistanceMatchingComponent = &DistanceMatchingComponents[Entity];
			
			//Start Distance Matching
			if (DistanceMatchingComponent->Type == 1)
			{
				//Set Start Distance Matching To Start Location
				DistanceMatchingComponent->StartLocation = DistanceMatchingComponent->InputLocation;

				//Add Debug
				if (CDebugFluidEntitiesCharacterHybridDistanceMatching.GetValueOnAnyThread() != 0)
					DrawDebugSphere(GetWorld(), DistanceMatchingComponent->InputLocation, 30, 3, FColor::Green, false, 1, 0, 1);

				//Reset Type
				DistanceMatchingComponent->Type = 0;
			}
			
			//Stop Distance Matching
			if (DistanceMatchingComponent->Type == 2)
			{
				//Velocity Variables
				SmallVelocity = 10.f * FMath::Square(DeltaTime);
				CurrentVelocityInFrame = DistanceMatchingComponent->InputVelocity * DeltaTime;
				CurrentVelocityDirection = DistanceMatchingComponent->InputVelocity.GetSafeNormal2D();
 
				// Deacceleration At Current Frame | Stop Frame Count
				DeaccelerationInFrame = (CurrentVelocityDirection * DistanceMatchingComponent->InputBrakingDeceleration) * FMath::Square(DeltaTime);
				StopFrameCountToZeroVelocity = CurrentVelocityInFrame.Size() / DeaccelerationInFrame.Size();

				//Set Starting Stopping Distance
				StoppingDistance = 0.0f;
 
				// Do Stop calculation go through all frames and calculate stop distance in each frame and stack them
				for (int j = 0; j <= StopFrameCountToZeroVelocity; j++)
				{
					//Update velocity
					CurrentVelocityInFrame -= DeaccelerationInFrame;
 
					// if velocity in XY plane is small break loop for safety
					if (CurrentVelocityInFrame.Size2D() <= SmallVelocity)
						break;
					
					// Calculate distance travel in current frame and add to previous distance
					StoppingDistance += CurrentVelocityInFrame.Size2D();
				}

				//Reset Type | Get Stop Location
				DistanceMatchingComponent->Type = 0;
				DistanceMatchingComponent->StopLocation = DistanceMatchingComponent->InputLocation + CurrentVelocityDirection * StoppingDistance;

				//Add Debug
				if (CDebugFluidEntitiesCharacterHybridDistanceMatching.GetValueOnAnyThread() != 0)
					DrawDebugSphere(GetWorld(), DistanceMatchingComponent->StopLocation, 30, 3, FColor::Red, false, 1, 0, 1);
			}
		}
	}
}

void UFluidEntitiesCharacterHybridSubsystem::EntitiesCharacterOrientationWarpingSystem(const float DeltaTime)
{
	//Return If State Is Disabled
	if (!EntitiesCharacterSystemStates[2])
		return;
	
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidEntitiesCharacter_OrientationWarping);
	{
		//Cache Repeated Variables (Better Performance)
		int Entity = 0;
		const float OrientationWarpingCardinalDirectionToAngleOffsets[4] = {0.0f, -90.0f, -180.0f, 90.0f};
		const float AngleMaxSkipDelta = 135.0f;
		float AngleMaxStepDelta = 45.0f;
		float OffsetDir = 0;
		float Angle = 0;
		float AngleOffset = 0;
		int CardinalDirection = 0;
		
		for (int i = 0; i < FLUID_ENTITIES_CORE_ENTITIES_MAX; i++)
		{
			//Read Data
			Entity = i;

			//Not Character Hybrid Entity So Skip
			if (!CharacterHybridEntitiesSignatures[Entity])
				continue;
			
			auto InputYawComponent = &InputYawComponents[Entity];
			auto InputStateComponent = &InputStateComponents[Entity];
			auto OrientationWarpingComponent = &OrientationWarpingComponents[Entity];

			//Only If Input Just Changed
			if (InputStateComponent->bJustChangedInput)
			{
				//Set Angle 
				Angle = InputYawComponent->YawDelta;

				//1 Cardinal Direction (Forward Animation)
				if (OrientationWarpingComponent->Type == 0)
					OrientationWarpingComponent->CardinalDirection = 0;
					
				//2 Cardinal Direction (Forward And Bck Animation)
				if (OrientationWarpingComponent->Type == 1)
				{
                    OrientationWarpingComponent->CardinalDirection = 0;
                    if (Angle > 135 || Angle < -45)
                    {
                         OrientationWarpingComponent->CardinalDirection = 2;
                         Angle += 180;
                    }
                    else
                         OrientationWarpingComponent->CardinalDirection = 0;
                }
			
				//4 Cardinal Direction (Forward, Back,Left,Right Animations)
				else if (OrientationWarpingComponent->Type == 2)
				{
					CardinalDirection = OrientationWarpingComponent->CardinalDirection;
					AngleMaxStepDelta = OrientationWarpingComponent->AngleMaxStepDelta;
					
					switch (CardinalDirection)
					{
					case 0: //Forward
						{
							if (Angle > AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = Angle  > AngleMaxSkipDelta ? 2 : 1;
							else if (Angle  < -AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = Angle  < -AngleMaxSkipDelta ? 2 : 3;
						} break;

					case 1: //Right
						{
							OffsetDir = Angle  - 90.0f;
							if (OffsetDir < -180.0f)
								OffsetDir += 360.0f;

							if (OffsetDir > AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = OffsetDir > AngleMaxSkipDelta ? 3 : 2;
							else if (OffsetDir < -AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = OffsetDir < -AngleMaxSkipDelta ? 3 : 0;
						} break;

					case 2: //Back
						{
							OffsetDir = Angle  - 180.0f;
							if (OffsetDir < -180.0f)
								OffsetDir += 360.0f;
							else if (OffsetDir > 180.0f)
								OffsetDir -= 360.0f;

							if (OffsetDir > AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = OffsetDir > AngleMaxSkipDelta ? 0 : 3;
							else if (OffsetDir < -AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = OffsetDir < -AngleMaxSkipDelta ? 0 : 1;
						} break;

					case 3: //Left
						{
							OffsetDir = Angle + 90;
							if (OffsetDir > 180.0f)
								OffsetDir -= 360.0f;

							if (OffsetDir > AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = OffsetDir > AngleMaxSkipDelta ? 1 : 0;
					
							else if (OffsetDir < -AngleMaxStepDelta)
								OrientationWarpingComponent->CardinalDirection = CardinalDirection = OffsetDir < -AngleMaxSkipDelta ? 1 : 2;
						} break;
					}

					//Get Angle Offset
					AngleOffset = OrientationWarpingCardinalDirectionToAngleOffsets[CardinalDirection];
				
					//Set Angle Based On Max Step Delta
					if (FMath::Abs(UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleDelta(-AngleOffset, UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleWrap(Angle))) <  AngleMaxStepDelta)
						Angle = UFluidEntitiesCharacterHybridBlueprintLibrary::MathGetAngleWrap(Angle + AngleOffset);
				}
							
				//Apply Angle (Use RInterp To Over FInterpTo As Rotation Goes Shortest Path)
				OrientationWarpingComponent->Angle = UKismetMathLibrary::RInterpTo(FRotator(0,OrientationWarpingComponent->Angle,0),FRotator(0,Angle,0),DeltaTime,OrientationWarpingComponent->InterpSpeed).Yaw;
				
				//Player Orientation Warping Component
				if (CDebugFluidEntitiesCharacterHybridOrientationWarping.GetValueOnAnyThread() != 0 && InputStateComponent->LocalPlayerIndex >= 0)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Type : " +  FString::FromInt(OrientationWarpingComponent->Type));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Cardinal Direction : " +  FString::FromInt(OrientationWarpingComponent->CardinalDirection));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Interp Speed : " +  FString::SanitizeFloat(OrientationWarpingComponent->InterpSpeed));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Angle Max Step Delta : " +  FString::SanitizeFloat(OrientationWarpingComponent->AngleMaxStepDelta));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,"Angle : " +  FString::SanitizeFloat(OrientationWarpingComponent->Angle));
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "Fluid Entities Character Hybrid Player 0 : Orientation Warping Component");
					GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, "");
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Systems State

void UFluidEntitiesCharacterHybridSubsystem::SetEntitiesSystemStates(bool bIsInputSystemEnabled,bool bIsDistanceMatchingSystemEnabled,bool bIsOrientationWarpingSystemEnabled, bool bIsInteractionSystemEnabled)
{
	SetEntitiesInputSystemState(bIsInputSystemEnabled);
	SetEntitiesDistanceMatchingSystemState(bIsDistanceMatchingSystemEnabled);
	SetEntitiesOrientationWarpingSystemStates(bIsOrientationWarpingSystemEnabled);
	SetEntitiesInteractionSystemState(bIsInteractionSystemEnabled);
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntitiesInputSystemState(bool bIsEnabled)
{
	EntitiesCharacterSystemStates[0] = bIsEnabled;
}


void UFluidEntitiesCharacterHybridSubsystem::SetEntitiesDistanceMatchingSystemState(bool bIsEnabled)
{
	EntitiesCharacterSystemStates[1] = bIsEnabled;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntitiesOrientationWarpingSystemStates(bool bIsEnabled)
{
	EntitiesCharacterSystemStates[2] = bIsEnabled;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntitiesInteractionSystemState(bool bIsEnabled)
{
	EntitiesCharacterSystemStates[3] = bIsEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Benchmark Subsystem Entities Signatures

void UFluidEntitiesCharacterHybridSubsystem::SetEntityToEntityCharacterHybridSignature(int Entity, bool bValue)
{
	CharacterHybridEntitiesSignatures[Entity] = bValue;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntitiesToEntityCharacterHybridSignature(TArray<int> Entities, bool bValue)
{
	for (int i = 0; i < Entities.Num(); i++)
		if (Entities[i] >= 0 && Entities[i] < FLUID_ENTITIES_CORE_ENTITIES_MAX)
			CharacterHybridEntitiesSignatures[Entities[i]] = bValue;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Set Components

void UFluidEntitiesCharacterHybridSubsystem::SetEntityTransformComponent(const int Entity, const FFluidEntitiesCharacterHybridTransformComponent Component)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Set Data
	TransformComponents[Entity] = Component;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityInputComponentInput(const int Entity, const float X, const float Y)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Set Data
	const auto Component = &InputXYComponents[Entity];
	Component->X = X;
	Component->Y = Y;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityInputStateComponentLocalPlayerIndex(const int Entity, const int LocalPlayerIndex)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;
    
    //Set Data
	const auto Component = &InputStateComponents[Entity];
    Component->LocalPlayerIndex = LocalPlayerIndex;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityInputStateComponentIsDisabled(const int Entity, const bool bIsDisabled)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;
    
	//Set Data
	const auto Component = &InputStateComponents[Entity];
	Component->bDisabled = bIsDisabled;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityDistanceMatchingComponentInput(const int Entity,const FVector Location, const FVector Velocity, const float BrakingDeceleration, const int Type)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Set Data
	const auto Component = &DistanceMatchingComponents[Entity];
	Component->InputLocation = Location;
	Component->InputVelocity = Velocity;
	Component->InputBrakingDeceleration = BrakingDeceleration;
	Component->Type = Type;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityOrientationWarpingComponentInput(const int Entity,const float InterpSpeed,const float AngleMaxStepDelta, const int Type)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;
	
	//Set Data
	const auto Component = &OrientationWarpingComponents[Entity];
	Component->InterpSpeed = InterpSpeed;
	Component->AngleMaxStepDelta = AngleMaxStepDelta;
	Component->Type = Type;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityMovementComponent(const int Entity, const FFluidEntitiesCharacterHybridMovementComponent Component)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
		return;

	//Set Data
	SetEntityMovementComponentUnsafe(Entity,Component);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Set Components Unsafe

void UFluidEntitiesCharacterHybridSubsystem::SetEntityTransformComponentUnsafe(const int Entity, const FFluidEntitiesCharacterHybridTransformComponent Component)
{
	TransformComponents[Entity] = Component;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityInputComponentInputUnsafe(const int Entity, const float X, const float Y)
{
	const auto Component = &InputXYComponents[Entity];
	Component->X = X;
	Component->Y = Y;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityInputStateComponentLocalPlayerIndexUnsafe(const int Entity, const int LocalPlayerIndex)
{
	const auto Component = &InputStateComponents[Entity];
    Component->LocalPlayerIndex = LocalPlayerIndex;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityInputStateComponentIsDisabledUnsafe(const int Entity, const bool bIsDisabled)
{
	const auto Component = &InputStateComponents[Entity];
	Component->bDisabled = bIsDisabled;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityDistanceMatchingComponentInputUnsafe(const int Entity,const FVector Location, const FVector Velocity, const float BrakingDeceleration, const int Type)
{
	const auto Component = &DistanceMatchingComponents[Entity];
	Component->InputLocation = Location;
	Component->InputVelocity = Velocity;
	Component->InputBrakingDeceleration = BrakingDeceleration;
	Component->Type = Type;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityOrientationWarpingComponentInputUnsafe(const int Entity,const float InterpSpeed,const float AngleMaxStepDelta, const int Type)
{
	const auto Component = &OrientationWarpingComponents[Entity];
	Component->InterpSpeed = InterpSpeed;
	Component->AngleMaxStepDelta = AngleMaxStepDelta;
	Component->Type = Type;
}

void UFluidEntitiesCharacterHybridSubsystem::SetEntityMovementComponentUnsafe(const int Entity, const FFluidEntitiesCharacterHybridMovementComponent Component)
{
	MovementComponents[Entity] = Component;

	//No Actor Binding So Continue
	const auto EntityCharacterBinding = EntityCharacterBindings[Entity];
	if (EntityCharacterBinding == nullptr)
		return;
	
	//Write Character Movement Component
	EntityCharacterBinding->GetCharacterMovement()->bOrientRotationToMovement = !Component.bStrafe;
	EntityCharacterBinding->GetCharacterMovement()->MaxWalkSpeed = Component.VelocityMaxWanted;
	EntityCharacterBinding->GetCharacterMovement()->MaxWalkSpeedCrouched = Component.VelocityMaxWanted;
	EntityCharacterBinding->GetCharacterMovement()->MaxFlySpeed = Component.VelocityMaxWanted;
	EntityCharacterBinding->GetCharacterMovement()->MaxSwimSpeed = Component.VelocityMaxWanted;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Get Components

void UFluidEntitiesCharacterHybridSubsystem::GetEntityTransformComponent(const int Entity, FFluidEntitiesCharacterHybridTransformComponent& Component_Out)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Component_Out = FFluidEntitiesCharacterHybridTransformComponent();
		return;
	}

	//Get Data
	Component_Out = TransformComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityInputXYComponent(const int Entity, FFluidEntitiesCharacterHybridInputXYComponent& Component_Out)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Component_Out = FFluidEntitiesCharacterHybridInputXYComponent();
		return;
	}

	//Get Data
	Component_Out = InputXYComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityInputYawComponent(const int Entity, FFluidEntitiesCharacterHybridInputYawComponent& Component_Out)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Component_Out = FFluidEntitiesCharacterHybridInputYawComponent();
		return;
	}

	//Get Data
	Component_Out = InputYawComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityInputStateComponent(const int Entity, FFluidEntitiesCharacterHybridInputStateComponent& Component_Out)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Component_Out = FFluidEntitiesCharacterHybridInputStateComponent();
		return;
	}
	
	//Get Data
	Component_Out = InputStateComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityDistanceMatchingComponent(const int Entity, FFluidEntitiesCharacterHybridDistanceMatchingComponent& Component_Out)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Component_Out = FFluidEntitiesCharacterHybridDistanceMatchingComponent();
		return;
	}

	//Get Data
	Component_Out = DistanceMatchingComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityOrientationWarpingComponent(const int Entity, FFluidEntitiesCharacterHybridOrientationWarpingComponent& Component_Out)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Component_Out = FFluidEntitiesCharacterHybridOrientationWarpingComponent();
		return;
	}

	//Get Data
	Component_Out = OrientationWarpingComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityMovementComponent(const int Entity, FFluidEntitiesCharacterHybridMovementComponent& Component_Out)
{
	//Don't Do If Out Of Bounds
	if (!bInitialized || Entity < 0 || Entity >= FLUID_ENTITIES_CORE_ENTITIES_MAX)
	{
		Component_Out = FFluidEntitiesCharacterHybridMovementComponent();
		return;
	}

	//Set Data
	Component_Out = MovementComponents[Entity];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Get Components Unsafe

void UFluidEntitiesCharacterHybridSubsystem::GetEntityTransformComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridTransformComponent& Component_Out)
{
	Component_Out = TransformComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityInputXYComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridInputXYComponent& Component_Out)
{
	Component_Out = InputXYComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityInputYawComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridInputYawComponent& Component_Out)
{
	Component_Out = InputYawComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityInputStateComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridInputStateComponent& Component_Out)
{
	Component_Out = InputStateComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityDistanceMatchingComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridDistanceMatchingComponent& Component_Out)
{
	Component_Out = DistanceMatchingComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityOrientationWarpingComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridOrientationWarpingComponent& Component_Out)
{
	Component_Out = OrientationWarpingComponents[Entity];
}

void UFluidEntitiesCharacterHybridSubsystem::GetEntityMovementComponentUnsafe(const int Entity, FFluidEntitiesCharacterHybridMovementComponent& Component_Out)
{
	Component_Out = MovementComponents[Entity];
}