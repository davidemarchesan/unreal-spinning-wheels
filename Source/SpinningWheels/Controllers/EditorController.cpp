// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/Input/Configs/EditorInputConfig.h"
#include "SpinningWheels/Pawns/EditorPawn.h"

void AEditorController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
}

void AEditorController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn && InPawn->IsA(AEditorPawn::StaticClass()))
	{
		EditorPawn = Cast<AEditorPawn>(InPawn);

		if (MainCamera.IsValid() == false)
		{
			CreateCamera();
		}
		if (IsLocalController() && EditorPawn.IsValid() && MainCamera.IsValid() && bCameraInitialized == true)
		{
			MainCamera->SetPawn(EditorPawn.Get());
		}
	}
}

void AEditorController::SetupInputBindings()
{
	SetupEditorInputBindings();
}

void AEditorController::SetupEditorInputBindings()
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
			
			EnhancedInput->BindAction(EditorInputConfig->IA_Move, ETriggerEvent::Triggered, this, &AEditorController::MoveCamera);
			EnhancedInput->BindAction(EditorInputConfig->IA_Rotate, ETriggerEvent::Triggered, this, &AEditorController::RotateCamera);
			
			// EnhancedInput->BindAction(EditorInputConfig->IA_F2, ETriggerEvent::Started, this, &ACarController::SwitchToDrive);
		}

	}
}

void AEditorController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Camera, false, Hit))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Cursor hit %s"), *Hit.ImpactPoint.ToString());
	}
}

void AEditorController::MoveCamera(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (EditorPawn.IsValid())
	{
		EditorPawn->Move(InputVector);
	}
}

void AEditorController::RotateCamera(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (EditorPawn.IsValid())
	{
		EditorPawn->Rotate(InputVector);
	}
}
