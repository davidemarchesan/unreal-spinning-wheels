// Fill out your copyright notice in the Description page of Project Settings.


#include "CarController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SpinningWheels/Input/Configs/DriveInputConfig.h"
#include "SpinningWheels/Pawns/Car.h"

void ACarController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (MappingContext)
			{
				InputSystem->AddMappingContext(MappingContext, 1);
			}
		}
	}
}

void ACarController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (InputConfig)
		{
			EnhancedInput->BindAction(InputConfig->IA_Drive, ETriggerEvent::Triggered, this, &ACarController::Drive);
			EnhancedInput->BindAction(InputConfig->IA_Brake, ETriggerEvent::Triggered, this, &ACarController::Brake);
			EnhancedInput->BindAction(InputConfig->IA_Turn, ETriggerEvent::Triggered, this, &ACarController::Turn);
		}
	}
}

void ACarController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	Car = Cast<ACar>(InPawn);
}

void ACarController::Drive()
{
	if (Car)
	{
		Car->Drive();
	}
}

void ACarController::Brake()
{
	UE_LOG(LogTemp, Warning, TEXT("ACarController::Brake"));
}

void ACarController::Turn()
{
	UE_LOG(LogTemp, Warning, TEXT("ACarController::Turn"));
}
