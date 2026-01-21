// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/Input/Configs/DriveInputConfig.h"
#include "SpinningWheels/Pawns/Car.h"

ARaceController::ARaceController()
{
	bAutoManageActiveCameraTarget = false;
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
			                          &ARaceController::StartDrive);
			EnhancedInput->BindAction(DriveInputConfig->IA_Drive, ETriggerEvent::Completed, this,
			                          &ARaceController::StopDrive);

			EnhancedInput->BindAction(DriveInputConfig->IA_Brake, ETriggerEvent::Started, this,
			                          &ARaceController::StartBrake);
			EnhancedInput->BindAction(DriveInputConfig->IA_Brake, ETriggerEvent::Completed, this,
			                          &ARaceController::StopBrake);

			EnhancedInput->BindAction(DriveInputConfig->IA_Turn, ETriggerEvent::Triggered, this,
			                          &ARaceController::Turn);

			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Started, this,
			                          &ARaceController::StartTurbo);
			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Completed, this,
			                          &ARaceController::StopTurbo);
		}
	}
}

void ARaceController::StartDrive()
{
	if (Car.IsValid())
	{
		Car->StartDrive();
	}
}

void ARaceController::StopDrive()
{
	if (Car.IsValid())
	{
		Car->StopDrive();
	}
}

void ARaceController::StartBrake()
{
	if (Car.IsValid())
	{
		Car->StartBrake();
	}
}

void ARaceController::StopBrake()
{
	if (Car.IsValid())
	{
		Car->StopBrake();
	}
}

void ARaceController::Turn(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (Car.IsValid())
	{
		Car->Turn(InputVector);
	}
}

void ARaceController::StartTurbo()
{
	if (Car.IsValid())
	{
		Car->StartTurbo();
	}
}

void ARaceController::StopTurbo()
{
	if (Car.IsValid())
	{
		Car->StopTurbo();
	}
}
