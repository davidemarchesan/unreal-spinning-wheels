// Fill out your copyright notice in the Description page of Project Settings.


#include "CarController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/Input/Configs/DriveInputConfig.h"
#include "SpinningWheels/Input/Configs/EditorInputConfig.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/Pawns/EditorPawn.h"

void ACarController::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCamera();
	SwitchToEditor();

}

void ACarController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ACarController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
}

void ACarController::SetupCamera()
{
	if (CameraClass)
	{
		MainCamera = GetWorld()->SpawnActor<AMainCamera>(CameraClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (MainCamera.IsValid())
		{
			SetViewTarget(MainCamera.Get());
		}
	}
}

void ACarController::StartDrive()
{
	if (Car.IsValid())
	{
		// Car->InputStartDrive();
	}
}

void ACarController::StopDrive()
{
	if (Car.IsValid())
	{
		// Car->InputStopDrive();
	}
}

void ACarController::StartBrake()
{
	if (Car.IsValid())
	{
		// Car->InputStartBrake();
	}
}

void ACarController::StopBrake()
{
	if (Car.IsValid())
	{
		// Car->InputStopBrake();
	}
}

void ACarController::Turn(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (Car.IsValid())
	{
		// Car->InputTurn(InputVector);
	}
}

void ACarController::StartTurbo()
{
	if (Car.IsValid())
	{
		// Car->InputStartTurbo();
	}
}

void ACarController::StopTurbo()
{
	if (Car.IsValid())
	{
		// Car->InputStopTurbo();
	}
}

void ACarController::SwitchToEditor()
{
	UE_LOG(LogTemp, Warning, TEXT("switch to editor"));

	if (Editor.IsValid() == false)
	{
		Editor = GetWorld()->SpawnActor<AEditorPawn>(EditorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	if (Editor.IsValid() && MainCamera.IsValid())
	{
		Possess(Editor.Get());
		MainCamera->SetPawn(Editor.Get());
		SetViewTarget(MainCamera.Get());
	}

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DriveMappingContext)
			{
				InputSystem->RemoveMappingContext(DriveMappingContext);
			}
			if (EditorMappingContext)
			{
				InputSystem->AddMappingContext(EditorMappingContext, 1);
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (EditorInputConfig)
		{
			EnhancedInput->ClearActionBindings();
			
			EnhancedInput->BindAction(EditorInputConfig->IA_Move, ETriggerEvent::Triggered, this, &ACarController::MoveCamera);
			EnhancedInput->BindAction(EditorInputConfig->IA_Rotate, ETriggerEvent::Triggered, this, &ACarController::RotateCamera);

			EnhancedInput->BindAction(EditorInputConfig->IA_F2, ETriggerEvent::Started, this, &ACarController::SwitchToDrive);
		}

	}
}

void ACarController::MoveCamera(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (Editor.IsValid())
	{
		Editor->Move(InputVector);
	}
}

void ACarController::RotateCamera(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (Editor.IsValid())
	{
		Editor->Rotate(InputVector);
	}
}

void ACarController::SwitchToDrive()
{
	UE_LOG(LogTemp, Warning, TEXT("switch to drive"));

	if (Car.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("car is not valid: spawn a new one"));
		Car = GetWorld()->SpawnActor<ACar>(CarClass, FVector(-3210.f,-690.f,160.f), FRotator::ZeroRotator);
	}

	if (Car.IsValid() && MainCamera.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("car is valid: set camera"));
		Possess(Car.Get());
		MainCamera->SetPawn(Car.Get());
		SetViewTarget(MainCamera.Get());
	}

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (EditorMappingContext)
			{
				InputSystem->RemoveMappingContext(EditorMappingContext);
			}
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
			
			EnhancedInput->BindAction(DriveInputConfig->IA_Drive, ETriggerEvent::Started, this, &ACarController::StartDrive);
			EnhancedInput->BindAction(DriveInputConfig->IA_Drive, ETriggerEvent::Completed, this,
									  &ACarController::StopDrive);

			EnhancedInput->BindAction(DriveInputConfig->IA_Brake, ETriggerEvent::Started, this, &ACarController::StartBrake);
			EnhancedInput->BindAction(DriveInputConfig->IA_Brake, ETriggerEvent::Completed, this,
									  &ACarController::StopBrake);

			EnhancedInput->BindAction(DriveInputConfig->IA_Turn, ETriggerEvent::Triggered, this, &ACarController::Turn);

			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Started, this, &ACarController::StartTurbo);
			EnhancedInput->BindAction(DriveInputConfig->IA_Turbo, ETriggerEvent::Completed, this,
									  &ACarController::StopTurbo);

			EnhancedInput->BindAction(DriveInputConfig->IA_F2, ETriggerEvent::Started, this, &ACarController::SwitchToEditor);
		}

	}
}
