// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/Actors/Blocks/Block.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "SpinningWheels/HUDs/EditorHUD.h"
#include "SpinningWheels/Input/Configs/EditorInputConfig.h"
#include "SpinningWheels/Pawns/EditorPawn.h"

void AEditorController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());


	HUD = GetHUD<AEditorHUD>();

	// test
	if (EditorBuildMenuData)
	{
		CurrentActiveMenu = FEditorBuildMenu(EditorBuildMenuData);

		if (HUD.IsValid())
		{
			HUD->InitializeBuildMenu(this, CurrentActiveMenu);
		}
	}
	// end test
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

			EnhancedInput->BindAction(EditorInputConfig->IA_Move, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputMoveCamera);
			EnhancedInput->BindAction(EditorInputConfig->IA_Rotate, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputRotateCamera);

			EnhancedInput->BindAction(EditorInputConfig->IA_Slot1, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot1);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot2, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot2);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot3, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot3);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot4, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot4);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot5, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot5);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot6, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot6);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot7, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot7);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot8, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot8);
			EnhancedInput->BindAction(EditorInputConfig->IA_Slot9, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputSlot9);
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

void AEditorController::InputMoveCamera(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (EditorPawn.IsValid())
	{
		EditorPawn->Move(InputVector);
	}
}

void AEditorController::InputRotateCamera(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	if (EditorPawn.IsValid())
	{
		EditorPawn->Rotate(InputVector);
	}
}

void AEditorController::InputSlot1()
{
	InputSlot(1);
}

void AEditorController::InputSlot2()
{
	InputSlot(2);
}

void AEditorController::InputSlot3()
{
	InputSlot(3);
}

void AEditorController::InputSlot4()
{
	InputSlot(4);
}

void AEditorController::InputSlot5()
{
	InputSlot(5);
}

void AEditorController::InputSlot6()
{
	InputSlot(6);
}

void AEditorController::InputSlot7()
{
	InputSlot(7);
}

void AEditorController::InputSlot8()
{
	InputSlot(8);
}

void AEditorController::InputSlot9()
{
	InputSlot(9);
}

void AEditorController::InputSlot(int8 Slot)
{

	const int8 Index = Slot - 1;
	if (CurrentActiveMenu.bInitialized == true)
	{
		if (CurrentActiveMenu.Items.IsValidIndex(Index))
		{
			const FEditorBuildMenuItem& Item = CurrentActiveMenu.Items[Index];
			OnMenuSlotSelected.Broadcast(Slot);
			
			if (Item.Submenu)
			{
				CurrentActiveMenu = FEditorBuildMenu(Item.Submenu);
				UE_LOG(LogTemp, Warning, TEXT("it's a submenu"));
			}
			else if (Item.Block)
			{
				UE_LOG(LogTemp, Warning, TEXT("it's a block %s"), *Item.Block->GetName());
			}
		}
	}
}
