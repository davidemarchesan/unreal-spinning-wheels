// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/Core/Simulation.h"
#include "SpinningWheels/GameModes/RaceGameMode.h"
#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/HUDs/RaceHUD.h"
#include "SpinningWheels/Input/Configs/DriveInputConfig.h"
#include "SpinningWheels/Input/Configs/GeneralInputConfig.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"

ARaceController::ARaceController()
{
	bAutoManageActiveCameraTarget = false;
}

void ARaceController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SimulatedTick(DeltaSeconds);

	StartDriveProcedure(DeltaSeconds);
}

void ARaceController::SimulatedTick(float DeltaSeconds)
{
	if (IsLocalController() == false)
	{
		return;
	}

	if (Phase != ERaceControllerPhase::RCP_Driving)
	{
		return;
	}

	AccSimulationTime += DeltaSeconds;
	TotSeconds += DeltaSeconds;

	if (AccSimulationTime >= SimulationConstants::TickFrequency)
	{
		int MaxIterations = FMath::Clamp(FMath::FloorToInt(AccSimulationTime / SimulationConstants::TickFrequency), 1,
		                                 30.f);
		int Iteration = 0;

		while (Iteration < MaxIterations)
		{
			Iteration++;

			FSimulationFrame SimulationFrame;
			SimulationFrame.DriveInputValue = DriveInputValue;
			SimulationFrame.BrakeInputValue = BrakeInputValue;
			SimulationFrame.TurnInputValue = TurnInputValue;
			SimulationFrame.TurboInputValue = TurboInputValue;

			if (RacePlayerState.IsValid())
			{
				RacePlayerState->AddSimulationFrame(SimulationFrame);
			}
		}

		LastSimulationDelta = AccSimulationTime - SimulationConstants::TickFrequency * MaxIterations;
		AccSimulationTime = LastSimulationDelta;

		TurnInputValue = 0;
	}
}

ARaceGameMode* ARaceController::GetRaceGameMode()
{
	if (HasAuthority() == false)
	{
		return nullptr;
	}

	return GetWorld()->GetAuthGameMode<ARaceGameMode>();
}

ARaceGameState* ARaceController::GetRaceGameState()
{
	return GetWorld()->GetGameState<ARaceGameState>();
}

ARacePlayerState* ARaceController::GetRacePlayerState()
{
	return GetPlayerState<ARacePlayerState>();
}

void ARaceController::TryGetRaceGameState()
{
	RaceGameState = GetRaceGameState();

	UE_LOG(LogTemp, Warning, TEXT("ARaceController::TryGetRaceGameState %d"), RaceGameState.IsValid());
	if (RaceGameState.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller, subscribed to RaceGameState->OnRaceMatchStateUpdate"));
		RaceGameState->OnRaceMatchStateUpdate.AddDynamic(this, &ARaceController::OnRaceMatchStateUpdate);
	}
}

void ARaceController::TryGetRacePlayerState()
{
	RacePlayerState = GetRacePlayerState();

	if (RacePlayerState.IsValid())
	{
		if (RaceHUD.IsValid())
		{
			RaceHUD->SetPlayerState(RacePlayerState.Get());
		}
		if (Car.IsValid())
		{
			Car->SetPlayerState(RacePlayerState.Get());
		}
	}
}

void ARaceController::TryGetRaceHUD()
{
	RaceHUD = GetHUD<ARaceHUD>();
	if (RaceHUD.IsValid() && RacePlayerState.IsValid())
	{
		RaceHUD->SetPlayerState(RacePlayerState.Get());
	}
}

void ARaceController::InputOpenMenu()
{
}

void ARaceController::BeginPlay()
{
	Super::BeginPlay();

	RacePlayerState = GetRacePlayerState();

	CreateCamera();

	EnableDefaultInputMappingContext();

	SetDefaultInputMode();
}

void ARaceController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn && InPawn->IsA(ACar::StaticClass()))
	{
		Car = Cast<ACar>(InPawn);

		if (MainCamera.IsValid() == false)
		{
			CreateCamera();
		}
		if (IsLocalController() && Car.IsValid() && MainCamera.IsValid() && bCameraInitialized == true)
		{
			MainCamera->SetPawn(Car.Get());
		}
	}
}

void ARaceController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetupInputBindings();
}

void ARaceController::BeginPlayingState()
{
	Super::BeginPlayingState();

	TryGetRaceGameState();
	TryGetRacePlayerState();
	TryGetRaceHUD();
}

void ARaceController::SetDefaultInputMode()
{
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void ARaceController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARaceController, Phase);
	DOREPLIFETIME(ARaceController, ServerStartDriveTime);
	DOREPLIFETIME(ARaceController, ServerRacingEndTime);
}

void ARaceController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	TryGetRacePlayerState();
}

void ARaceController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Car.IsValid() && RacePlayerState.IsValid())
	{
		Car->SetPlayerState(RacePlayerState.Get());
	}

	if (RacePlayerState.IsValid() == false)
	{
		TryGetRacePlayerState();
	}
}

void ARaceController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);

	bCameraInitialized = false;

	DriveInputValue = 0;
	BrakeInputValue = 0;
	TurnInputValue = 0;
	TurboInputValue = 0;

	if (RaceHUD.IsValid())
	{
		RaceHUD->ClearViewport();
	}

	if (RaceGameState.IsValid())
	{
		RaceGameState->OnRaceMatchStateUpdate.RemoveDynamic(this, &ARaceController::OnRaceMatchStateUpdate);
	}

	RaceHUD.Reset();
	RacePlayerState.Reset();
	RaceGameState.Reset();
}

void ARaceController::SetPhase(ERaceControllerPhase NewPhase)
{
	Phase = NewPhase;

	if (HasAuthority())
	{
		InternalSetPhase(NewPhase);
	}
	else
	{
		ServerSetPhase(NewPhase);;
	}
}

void ARaceController::InternalSetPhase(ERaceControllerPhase NewPhase)
{
	Phase = NewPhase;
}

void ARaceController::ServerSetPhase_Implementation(ERaceControllerPhase NewPhase)
{
	InternalSetPhase(NewPhase);
}

void ARaceController::PrepareForNewLap(const float InServerStartTime)
{
	if (HasAuthority() == false)
	{
		return;
	}

	SetPhase(ERaceControllerPhase::RCP_InStartingProcedure);
	ServerStartDriveTime = InServerStartTime;

	// Udate match timer
	if (RaceGameState.IsValid())
	{
		SetRacingEndTime(RaceGameState->GetServerRacingEndTime());
	}
}

void ARaceController::SetRacingEndTime(const float InServerRacingEndTime)
{
	ServerRacingEndTime = InServerRacingEndTime;

	OnRep_ServerRacingEndTime();
}

void ARaceController::SetupInputBindings()
{
	SetupDriveInputBindings();
	SetupGeneralInputBindings();
}

void ARaceController::SetupDriveInputBindings()
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (DriveInputConfig)
		{
			EnhancedInput->BindAction(DriveInputConfig->IA_Drive, ETriggerEvent::Started, this,
			                          &ARaceController::InputStartDrive);
			EnhancedInput->BindAction(DriveInputConfig->IA_Drive, ETriggerEvent::Completed, this,
			                          &ARaceController::InputStopDrive);

			EnhancedInput->BindAction(DriveInputConfig->IA_Brake, ETriggerEvent::Started, this,
			                          &ARaceController::InputStartBrake);
			EnhancedInput->BindAction(DriveInputConfig->IA_Brake, ETriggerEvent::Completed, this,
			                          &ARaceController::InputStopBrake);

			EnhancedInput->BindAction(DriveInputConfig->IA_Turn, ETriggerEvent::Triggered, this,
			                          &ARaceController::InputTurn);

			EnhancedInput->BindAction(DriveInputConfig->IA_CancelLap, ETriggerEvent::Triggered, this,
			                          &ARaceController::InputCancelLap);

			EnhancedInput->BindAction(DriveInputConfig->IA_Leaderboard, ETriggerEvent::Started, this,
			                          &ARaceController::InputShowLeaderboard);
			EnhancedInput->BindAction(DriveInputConfig->IA_Leaderboard, ETriggerEvent::Completed, this,
			                          &ARaceController::InputHideLeaderboard);

			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Started, this,
			                          &ARaceController::InputStartTurbo);
			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Completed, this,
			                          &ARaceController::InputStopTurbo);

			EnhancedInput->BindAction(DriveInputConfig->IA_Debug, ETriggerEvent::Triggered, this,
			                          &ARaceController::Debug);
		}
	}
}

void ARaceController::SetupGeneralInputBindings()
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (GeneralInputConfig)
		{
			EnhancedInput->BindAction(GeneralInputConfig->IA_OpenMenu, ETriggerEvent::Triggered, this,
			                          &ARaceController::InputOpenMenu);
		}
	}
}

void ARaceController::EnableDefaultInputMappingContext()
{
	EnableDriveInputMappingContext();
	EnableGeneralInputMappingContext();
}

void ARaceController::EnableDriveInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DriveMappingContext)
			{
				InputSystem->AddMappingContext(DriveMappingContext, 10);
			}
		}
	}
}

void ARaceController::DisableDriveInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DriveMappingContext)
			{
				InputSystem->RemoveMappingContext(DriveMappingContext);
			}
		}
	}
}

void ARaceController::EnableGeneralInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (GeneralMappingContext)
			{
				InputSystem->AddMappingContext(GeneralMappingContext, 1);
			}
		}
	}
}

void ARaceController::DisableGeneralInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (GeneralMappingContext)
			{
				InputSystem->RemoveMappingContext(GeneralMappingContext);
			}
		}
	}
}

void ARaceController::CreateCamera()
{
	if (CanCreateCamera() == false)
	{
		return;
	}

	bCameraInitializing = true;

	if (CameraClass)
	{
		MainCamera = GetWorld()->SpawnActor<AMainCamera>(CameraClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (MainCamera.IsValid())
		{
			SetViewTarget(MainCamera.Get());
			bCameraInitialized = true;
		}
	}

	bCameraInitializing = false;
}

void ARaceController::OnRep_Phase()
{
}

void ARaceController::OnRep_ServerRacingEndTime()
{
	if (AGameState* RGS = GetRaceGameState())
	{
		const float CurrentServerTime = RGS->GetServerWorldTimeSeconds();
		const float RemainingTime = ServerRacingEndTime - CurrentServerTime;

		if (RaceHUD.IsValid())
		{
			RaceHUD->SetMatchRemainingTime(RemainingTime);
		}
	}
}

void ARaceController::StartDriveProcedure(float DeltaSeconds)
{
	if (IsLocalController() == false)
	{
		return;
	}

	if (Phase != ERaceControllerPhase::RCP_InStartingProcedure)
	{
		return;
	}

	if (ServerStartDriveTime == 0.f)
	{
		return;
	}

	if (RaceGameState.IsValid())
	{
		float CurrentServerTime = RaceGameState->GetServerWorldTimeSeconds();

		if (ServerStartDriveTime <= CurrentServerTime && Phase == ERaceControllerPhase::RCP_InStartingProcedure)
		{
			StartLap();
			return;
		}

		// Update UI
		const float Diff = FMath::CeilToInt(ServerStartDriveTime - CurrentServerTime);

		if (Diff != StartDriveSecondsRemaining)
		{
			StartDriveSecondsRemaining = Diff;
			if (RaceHUD.IsValid())
			{
				RaceHUD->UpdateLapCountdown(StartDriveSecondsRemaining);
			}
		}
	}
}

void ARaceController::InputStartDrive()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	DriveInputValue = 1;
}

void ARaceController::InputStopDrive()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	DriveInputValue = 0;
}

void ARaceController::InputStartBrake()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	BrakeInputValue = 1;
}

void ARaceController::InputStopBrake()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	BrakeInputValue = 0;
}

void ARaceController::InputTurn(const FInputActionValue& Value)
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	FVector2D InputVector = Value.Get<FVector2D>();
	TurnInputValue = InputVector.Y;
}

void ARaceController::InputStartTurbo()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	TurboInputValue = 1;
}

void ARaceController::InputStopTurbo()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	TurboInputValue = 0;
}

void ARaceController::InputCancelLap()
{
	if (Phase != ERaceControllerPhase::RCP_Driving)
	{
		return;
	}

	if (IsLocalController() && HasAuthority() == true)
	{
		// Server player only
		InternalCancelLap();
	}
	else
	{
		// Ask the server to restart me
		ServerCancelLap();
	}

	if (RacePlayerState.IsValid())
	{
		RacePlayerState->CancelLap();
	}
}

void ARaceController::InputShowLeaderboard()
{
	if (RaceHUD.IsValid())
	{
		RaceHUD->ShowLeaderboard();
	}
}

void ARaceController::InputHideLeaderboard()
{
	if (RaceHUD.IsValid())
	{
		RaceHUD->HideLeaderboard();
	}
}

void ARaceController::InternalCancelLap()
{
	if (ARaceGameModeBase* GM = GetWorld()->GetAuthGameMode<ARaceGameModeBase>())
	{
		GM->CancelLap(this);
	}
}

void ARaceController::ServerCancelLap_Implementation()
{
	InternalCancelLap();
}

void ARaceController::StartLap()
{
	if (RacePlayerState.IsValid() == false)
	{
		TryGetRacePlayerState();
	}

	if (IsLocalController())
	{
		// Server-player or client-predict
		LocalStartLap();
	}
}

void ARaceController::LocalStartLap()
{
	SetPhase(ERaceControllerPhase::RCP_Driving);

	if (RacePlayerState.IsValid())
	{
		RacePlayerState->OnStartLap();
	}

	StartDriveSecondsRemaining = 0;
	if (RaceHUD.IsValid())
	{
		RaceHUD->UpdateLapCountdown(StartDriveSecondsRemaining);
	}
}

void ARaceController::ServerStartLap_Implementation()
{
	SetPhase(ERaceControllerPhase::RCP_Driving);
	StartDriveSecondsRemaining = 0;
}

void ARaceController::OnRaceMatchStateUpdate(ERaceMatchState NewState)
{
	switch (NewState)
	{
	case ERaceMatchState::RMS_Podium:
		SetPhase(ERaceControllerPhase::RCP_Respawning);
		if (RacePlayerState.IsValid())
		{
			RacePlayerState->Stop();
		}
		break;

	default:
		break;
	}
}

void ARaceController::Debug()
{
}
