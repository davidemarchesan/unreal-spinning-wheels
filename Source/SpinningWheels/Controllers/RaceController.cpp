// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/GameModes/RaceGameMode.h"
#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/Input/Configs/DriveInputConfig.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"

ARaceController::ARaceController()
{
	bAutoManageActiveCameraTarget = false;
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

	DisableInput(this); // GameMode will enable me when map and players are ready

	SetupDriveInputBindings();

	SetupCamera();


	// if (PlayerState)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("ARaceController: beginplay! (role %d) (pid %d)"), GetLocalRole(),
	// 	       PlayerState->GetPlayerId());
	// }
}

void ARaceController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ARaceController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	Car = Cast<ACar>(InPawn);

	SetupCamera();

	// if (PlayerState && InPawn)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("ARaceController: set pawn! (role %d) (pid %d) (pawn class %s"), GetLocalRole(),
	// 		   PlayerState->GetPlayerId(), *InPawn->GetClass()->GetName());
	// }
}

void ARaceController::SetupCamera()
{
	if (bCameraInitialized == true)
	{
		return;
	}

	if (IsLocalController() && CameraClass)
	{
		MainCamera = GetWorld()->SpawnActor<AMainCamera>(CameraClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (MainCamera.IsValid() && Car.IsValid())
		{
			MainCamera->SetPawn(Car.Get());
			SetViewTarget(MainCamera.Get());

			bCameraInitialized = true;
		}
	}
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
		}
	}
}

void ARaceController::InputStartDrive()
{
	if (Car.IsValid())
	{
		Car->InputStartDrive();
	}
}

void ARaceController::InputStopDrive()
{
	if (Car.IsValid())
	{
		Car->InputStopDrive();
	}
}

void ARaceController::InputStartBrake()
{
	if (Car.IsValid())
	{
		Car->InputStartBrake();
	}
}

void ARaceController::InputStopBrake()
{
	if (Car.IsValid())
	{
		Car->InputStopBrake();
	}
}

void ARaceController::InputTurn(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (Car.IsValid())
	{
		Car->InputTurn(InputVector);
	}
}

void ARaceController::InputStartTurbo()
{
	if (Car.IsValid())
	{
		Car->InputStartTurbo();
	}
}

void ARaceController::InputStopTurbo()
{
	if (Car.IsValid())
	{
		Car->InputStopTurbo();
	}
}

void ARaceController::InputCancelLap()
{
	// Ask the server to restart me
	if (IsLocalController() && HasAuthority() == false)
	{
		ServerCancelLap();
	}

	// Ask the server to start a new lap
	// ServerCancelLap();

	// todo: Testing
	// if (ARacePlayerState* PS = GetRacePlayerState())
	// {
	// 	PS->OnStartLap();
	// }
}

void ARaceController::ServerCancelLap_Implementation()
{
	if (ARaceGameMode* GM = GetRaceGameMode())
	{
		GM->CancelLap(this);
	}
}
