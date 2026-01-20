// Fill out your copyright notice in the Description page of Project Settings.


#include "CarController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/Input/Configs/DriveInputConfig.h"
#include "SpinningWheels/Pawns/Car.h"

void ACarController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (MappingContext)
			{
				InputSystem->AddMappingContext(MappingContext, 1);
			}
		}
	}

	SetupCamera();
}

void ACarController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (InputConfig)
		{
			EnhancedInput->BindAction(InputConfig->IA_Drive, ETriggerEvent::Started, this, &ACarController::StartDrive);
			EnhancedInput->BindAction(InputConfig->IA_Drive, ETriggerEvent::Completed, this,
			                          &ACarController::StopDrive);

			EnhancedInput->BindAction(InputConfig->IA_Brake, ETriggerEvent::Started, this, &ACarController::StartBrake);
			EnhancedInput->BindAction(InputConfig->IA_Brake, ETriggerEvent::Completed, this,
			                          &ACarController::StopBrake);

			EnhancedInput->BindAction(InputConfig->IA_Turn, ETriggerEvent::Triggered, this, &ACarController::Turn);

			EnhancedInput->BindAction(InputConfig->IA_Turbo, ETriggerEvent::Started, this, &ACarController::StartTurbo);
			EnhancedInput->BindAction(InputConfig->IA_Turbo, ETriggerEvent::Completed, this,
			                          &ACarController::StopTurbo);
		}
	}
}

void ACarController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	Car = Cast<ACar>(InPawn);
}

void ACarController::SetupCamera()
{
	if (CameraClass)
	{
		MainCamera = GetWorld()->SpawnActor<AMainCamera>(CameraClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (MainCamera.IsValid())
		{
			SetViewTarget(MainCamera.Get());
			if (Car.IsValid())
			{
				MainCamera->SetPawn(Car.Get());
			}
		}
	}
}

void ACarController::StartDrive()
{
	if (Car.IsValid())
	{
		Car->StartDrive();
	}
}

void ACarController::StopDrive()
{
	if (Car.IsValid())
	{
		Car->StopDrive();
	}
}

void ACarController::StartBrake()
{
	if (Car.IsValid())
	{
		Car->StartBrake();
	}
}

void ACarController::StopBrake()
{
	if (Car.IsValid())
	{
		Car->StopBrake();
	}
}

void ACarController::Turn(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (Car.IsValid())
	{
		Car->Turn(InputVector);
	}
}

void ACarController::StartTurbo()
{
	if (Car.IsValid())
	{
		Car->StartTurbo();
	}
}

void ACarController::StopTurbo()
{
	if (Car.IsValid())
	{
		Car->StopTurbo();
	}
}
