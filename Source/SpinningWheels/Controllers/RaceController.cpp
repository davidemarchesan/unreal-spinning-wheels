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
	SetupCamera();
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
}

void ARaceController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARaceController, bCanDrive);
	DOREPLIFETIME(ARaceController, ServerStartDriveTime);
}

void ARaceController::PrepareForNewLap(float InServerStartTime)
{

	if (HasAuthority() == false)
	{
		return;
	}

	bCanDrive = false;
	ServerStartDriveTime = InServerStartTime;
}

void ARaceController::SetCanDrive(bool bInCanDrive)
{
	bCanDrive = bInCanDrive;
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

			EnhancedInput->BindAction(DriveInputConfig->IA_Debug, ETriggerEvent::Triggered, this,
			                          &ARaceController::Debug);
		}
	}
}

void ARaceController::StartDriveProcedure(float DeltaSeconds)
{

	if (HasAuthority() == true)
	{
		return;
	}
	
	if (bCanDrive == true || ServerStartDriveTime == 0.f)
	{
		return;
	}

	if (ARaceGameState* RGS = GetRaceGameState())
	{

		float CurrentServerTime = RGS->GetServerWorldTimeSeconds();
		
		if (ServerStartDriveTime <= CurrentServerTime && bCanDrive == false)
		{
			StartLap();
			return;
		}

		const float Diff = FMath::CeilToInt(ServerStartDriveTime - CurrentServerTime);

		if (Diff != StartDriveSecondsRemaining)
		{
			StartDriveSecondsRemaining = Diff;
			OnUpdateLapCountdown.Broadcast(StartDriveSecondsRemaining);
			UE_LOG(LogTemp, Warning, TEXT("(role %d) Remaining seconds %d"), GetLocalRole(), StartDriveSecondsRemaining);
		}
		
	}
}

void ARaceController::InputStartDrive()
{
	if (Car.IsValid() && bCanDrive)
	{
		Car->InputStartDrive();
	}
}

void ARaceController::InputStopDrive()
{
	if (Car.IsValid() && bCanDrive)
	{
		Car->InputStopDrive();
	}
}

void ARaceController::InputStartBrake()
{
	if (Car.IsValid() && bCanDrive)
	{
		Car->InputStartBrake();
	}
}

void ARaceController::InputStopBrake()
{
	if (Car.IsValid() && bCanDrive)
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
	if (Car.IsValid() && bCanDrive)
	{
		Car->InputStartTurbo();
	}
}

void ARaceController::InputStopTurbo()
{
	if (Car.IsValid() && bCanDrive)
	{
		Car->InputStopTurbo();
	}
}

void ARaceController::InputCancelLap()
{
	if (bCanDrive == false)
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
	UE_LOG(LogTemp, Warning, TEXT("ARaceController::StartLap xx"));
	SetCanDrive(true);
	
	StartDriveSecondsRemaining = 0;
	OnUpdateLapCountdown.Broadcast(StartDriveSecondsRemaining);
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
