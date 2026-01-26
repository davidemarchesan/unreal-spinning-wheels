// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/GameModes/RaceGameMode.h"
#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"
#include "SpinningWheels/Input/Configs/DriveInputConfig.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"

ARaceController::ARaceController()
{
	bAutoManageActiveCameraTarget = false;
}

void ARaceController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	StartDriveProcedure(DeltaSeconds);
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

void ARaceController::BeginPlay()
{
	Super::BeginPlay();

	SetupDriveInputBindings();
	CreateCamera();
}

void ARaceController::SetupInputComponent()
{
	Super::SetupInputComponent();
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

void ARaceController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARaceController, Phase);
	DOREPLIFETIME(ARaceController, ServerStartDriveTime);
}

void ARaceController::SetPhase(ERaceControllerPhase NewPhase)
{
	Phase = NewPhase;
}

void ARaceController::PrepareForNewLap(float InServerStartTime)
{
	if (HasAuthority() == false)
	{
		return;
	}

	
	SetPhase(ERaceControllerPhase::RCP_InStartingProcedure);
	ServerStartDriveTime = InServerStartTime;
	
}

void ARaceController::SetupDriveInputBindings()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DriveMappingContext)
			{
				InputSystem->AddMappingContext(DriveMappingContext, 1);
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (DriveInputConfig)
		{
			EnhancedInput->ClearActionBindings();

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

			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Started, this,
			                          &ARaceController::InputStartTurbo);
			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Completed, this,
			                          &ARaceController::InputStopTurbo);

			EnhancedInput->BindAction(DriveInputConfig->IA_Debug, ETriggerEvent::Triggered, this,
			                          &ARaceController::Debug);
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
	if (Phase == ERaceControllerPhase::RCP_Driving)
	{
		LocalStartLap();
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

	if (ARaceGameState* RGS = GetRaceGameState())
	{
		float CurrentServerTime = RGS->GetServerWorldTimeSeconds();
 
		if (ServerStartDriveTime <= CurrentServerTime && Phase == ERaceControllerPhase::RCP_InStartingProcedure)
		{
			LocalStartLap();
			return;
		}
 
		// Update UI
		const float Diff = FMath::CeilToInt(ServerStartDriveTime - CurrentServerTime);

		if (Diff != StartDriveSecondsRemaining)
		{
			StartDriveSecondsRemaining = Diff;
			OnUpdateLapCountdown.Broadcast(StartDriveSecondsRemaining);
		}
			
		
	}
}

void ARaceController::InputStartDrive()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	if (Car.IsValid())
	{
		Car->InputStartDrive();
	}
}

void ARaceController::InputStopDrive()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	if (Car.IsValid())
	{
		Car->InputStopDrive();
	}
}

void ARaceController::InputStartBrake()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	if (Car.IsValid())
	{
		Car->InputStartBrake();
	}
}

void ARaceController::InputStopBrake()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	if (Car.IsValid())
	{
		Car->InputStopBrake();
	}
}

void ARaceController::InputTurn(const FInputActionValue& Value)
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	FVector2D InputVector = Value.Get<FVector2D>();
	if (Car.IsValid())
	{
		Car->InputTurn(InputVector);
	}
}

void ARaceController::InputStartTurbo()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	if (Car.IsValid())
	{
		Car->InputStartTurbo();
	}
}

void ARaceController::InputStopTurbo()
{
	if (Phase == ERaceControllerPhase::RCP_Respawning)
	{
		return;
	}

	if (Car.IsValid())
	{
		Car->InputStopTurbo();
	}
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
		if (ARaceGameMode* GM = GetRaceGameMode())
		{
			GM->CancelLap(this);
		}
	}
	else
	{
		// Ask the server to restart me
		ServerCancelLap();
	}
}

void ARaceController::StartLap()
{
	if (IsLocalController())
	{
		// Server-player or client-predict
		LocalStartLap();
	}

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Tells server
		ServerStartLap(); 
	}
}

void ARaceController::LocalStartLap()
{
	if (Car.IsValid())
	{
		Car->LocalStartEngine();

		SetPhase(ERaceControllerPhase::RCP_Driving);

		StartDriveSecondsRemaining = 0;
		OnUpdateLapCountdown.Broadcast(StartDriveSecondsRemaining);
	}
}

void ARaceController::ServerStartLap_Implementation()
{
	LocalStartLap();
}

void ARaceController::ServerCancelLap_Implementation()
{
	if (ARaceGameMode* GM = GetRaceGameMode())
	{
		GM->CancelLap(this);
	}
}

void ARaceController::Debug()
{
	FMainStyle::Shutdown();
	FMainStyle::Initialize();
}
