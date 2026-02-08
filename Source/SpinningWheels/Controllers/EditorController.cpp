// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SpinningWheels/Actors/MainCamera.h"
#include "SpinningWheels/Actors/TrackGrid.h"
#include "SpinningWheels/Actors/Blocks/Block.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "SpinningWheels/GameModes/EditorGameMode.h"
#include "SpinningWheels/HUDs/EditorHUD.h"
#include "SpinningWheels/Input/Configs/BuildInputConfig.h"
#include "SpinningWheels/Input/Configs/EditorInputConfig.h"
#include "SpinningWheels/Pawns/EditorPawn.h"

void AEditorController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());

	GameMode = GetWorld()->GetAuthGameMode<AEditorGameMode>();
	SetupTrackGrid();

	HUD = GetHUD<AEditorHUD>();

	SetupBuildMenu();
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

		MovePawnAtCenter();
	}
}

void AEditorController::SetupInputBindings()
{
	Super::SetupInputBindings();

	SetupEditorInputBindings();
	SetupBuildInputBindings();
}

void AEditorController::SetupEditorInputBindings()
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (EditorInputConfig)
		{

			EnhancedInput->BindAction(EditorInputConfig->IA_Move, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputMoveCamera);
			EnhancedInput->BindAction(EditorInputConfig->IA_Rotate, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputRotateCamera);

			EnhancedInput->BindAction(EditorInputConfig->IA_SelectBlock, ETriggerEvent::Triggered, this,
									  &AEditorController::InputSelectBlock);
			EnhancedInput->BindAction(EditorInputConfig->IA_RemoveBlock, ETriggerEvent::Triggered, this,
									  &AEditorController::InputRemoveBlock);

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

void AEditorController::SetupBuildInputBindings()
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (EditorBuildInputConfig)
		{
			EnhancedInput->BindAction(EditorBuildInputConfig->IA_Build, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputBuildBlock);
			EnhancedInput->BindAction(EditorBuildInputConfig->IA_Cancel, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputBuildCancel);
			EnhancedInput->BindAction(EditorBuildInputConfig->IA_Rotate, ETriggerEvent::Triggered, this,
			                          &AEditorController::InputBuildRotateBlock);
		}
	}
}

void AEditorController::EnableDefaultInputMappingContext()
{
	EnableEditorInputMappingContext();
}

void AEditorController::EnableEditorInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (EditorMappingContext)
			{
				InputSystem->AddMappingContext(EditorMappingContext, 1);
			}
		}
	}
}

void AEditorController::DisableEditorInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (EditorMappingContext)
			{
				InputSystem->RemoveMappingContext(EditorMappingContext);
			}
		}
	}
}

void AEditorController::EnableBuildInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (EditorBuildMappingContext)
			{
				InputSystem->AddMappingContext(EditorBuildMappingContext, 1);
			}
		}
	}
}

void AEditorController::DisableBuildInputMappingContext()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (EditorBuildMappingContext)
			{
				InputSystem->RemoveMappingContext(EditorBuildMappingContext);
			}
		}
	}
}

void AEditorController::SetupTrackGrid()
{
	if (GameMode.IsValid())
	{
		TrackGrid = GameMode->GetTrackGrid();
		if (TrackGrid.IsValid() == false)
		{
			GameMode->OnTrackGridReady.AddDynamic(this, &AEditorController::OnTrackGridReady);
		}
	}
}

void AEditorController::BlockCursor()
{
	SetInputMode(FInputModeUIOnly());
	bIgnoreInput = true;
}

void AEditorController::UnlockCursor()
{
	SetInputMode(FInputModeGameAndUI());
	bIgnoreInput = false;
}

void AEditorController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PreviewBlock();
	HoverBlock();
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

void AEditorController::InputSelectBlock()
{
	if (HoveredBlock.IsValid() == false)
	{
		return;
	}
	
	if (TrackGrid->Remove(HoveredBlock->GetActorLocation()))
	{
		EnterBuildModeWithHovered();
	}
	
}

void AEditorController::InputRemoveBlock()
{
	if (HoveredBlock.IsValid() == false || TrackGrid.IsValid() == false)
	{
		return;
	}

	if (TrackGrid->Remove(HoveredBlock->GetActorLocation()))
	{
		HoveredBlock->Destroy();
		HoveredBlock = nullptr;
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

void AEditorController::InputBuildBlock()
{
	if (bBuildMode == false)
	{
		return;
	}

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit))
	{
		if (TrackGrid.IsValid() && BlockToBuildName.IsValid() && PreviewedBlock.IsValid())
		{
			TrackGrid->Build(BlockToBuildName, Hit.ImpactPoint, PreviewedBlock->GetActorRotation());
		}
	}
}

void AEditorController::InputBuildCancel()
{
	ExitBuildMode();
}

void AEditorController::InputBuildRotateBlock(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	if (PreviewedBlock.IsValid() && PreviewedBlock->IsActorBeingDestroyed() == false)
	{
		const float Rotation = InputVector.X * -1;

		FRotator CurrentRotation = PreviewedBlock->GetActorRotation();
		PreviewedBlock->SetActorRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + Rotation * 90.f,
		                                          CurrentRotation.Roll));
	}
}

void AEditorController::PreviewBlock()
{
	if (bBuildMode == false || TrackGrid.IsValid() == false || PreviewedBlock.IsValid() == false)
	{
		return;
	}

	// Get coordinates and world location where im pointing to
	// move the previewed block there (spawns it if null)
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit))
	{
		if (TrackGrid->CanBuildOn(Hit.ImpactPoint))
		{
			FVector Location = TrackGrid->GetTileWorldLocation(Hit.ImpactPoint);
			if (PreviewedBlock->GetActorLocation() != Location)
			{
				PreviewedBlock->SetActorLocation(Location);
				PreviewedBlock->SetActorHiddenInGame(false);
			}
		}
		else
		{
			PreviewedBlock->SetActorHiddenInGame(true);
			PreviewedBlock->SetActorLocation(FVector::ZeroVector);
		}
	}
}

void AEditorController::HoverBlock()
{
	if (bBuildMode == true || bIgnoreInput == true)
	{
		return;
	}

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel2, false, Hit))
	{
		if (Hit.GetActor())
		{
			if (HoveredBlock.IsValid())
			{
				if (Hit.GetActor() == HoveredBlock.Get())
				{
					// Player is hovering the same actor
					return;
				}
				else
				{
					// It'a new block, so hide the old one
					HoveredBlock->HideOutline();
				}
			}

			if (ABlock* Block = Cast<ABlock>(Hit.GetActor()))
			{
				HoveredBlock = Block;
				if (HoveredBlock.IsValid())
				{
					HoveredBlock->ShowOutline();
				}
			}
		}
	}
	else
	{
		if (HoveredBlock.IsValid())
		{
			HoveredBlock->HideOutline();
			HoveredBlock = nullptr;
		}
	}
}

void AEditorController::OnTrackGridReady(ATrackGrid* InTrackGrid)
{
	if (InTrackGrid)
	{
		TrackGrid = MakeWeakObjectPtr<ATrackGrid>(InTrackGrid);

		if (TrackGrid.IsValid())
		{
			MovePawnAtCenter();
		}
	}
}

void AEditorController::MovePawnAtCenter()
{
	if (TrackGrid.IsValid() == false || EditorPawn.IsValid() == false)
	{
		return;
	}

	EditorPawn->SetActorLocation(TrackGrid->GetWorldCenter());
}

void AEditorController::SetupBuildMenu()
{
	if (EditorBuildMenuData)
	{
		CurrentActiveMenu = FEditorBuildMenu(EditorBuildMenuData);
	}
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
			}
			else if (Item.BlocksTableRow.IsNull() == false)
			{
				if (FBlockRow* BlockRow = Item.BlocksTableRow.GetRow<FBlockRow>("Preview"))
				{
					EnterBuildMode(Item.BlocksTableRow.RowName, *BlockRow);
				}
			}
		}
	}
}

void AEditorController::InputSelectMenu(UEditorBuildMenuDataAsset* Menu)
{
	if (Menu)
	{
		CurrentActiveMenu = FEditorBuildMenu(Menu);
	}
}

void AEditorController::InputTestTrack()
{
	UE_LOG(LogTemp, Warning, TEXT("controller test track"));

	if (GameMode.IsValid())
	{
		GameMode->TestTrack(this);

		DisableEditorInputMappingContext();
		EnableDriveInputMappingContext();

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}

	// switch control input
	// spawn pawn
	// set pawn control
	// camera set to car pawn
}

void AEditorController::InputSaveTrack(const FString& TrackName)
{
	if (GameMode.IsValid())
	{
		GameMode->SaveTrack(TrackName);
	}
}

FString AEditorController::GetTrackName()
{
	if (GameMode.IsValid())
	{
		return GameMode->GetTrackName();
	}

	return "";
}

void AEditorController::EnterBuildMode(const FName& RowName, const FBlockRow& BlockRow)
{
	if (TrackGrid.IsValid() == false)
	{
		return;
	}

	if (bBuildMode == false)
	{
		bBuildMode = true;
		EnableBuildInputMappingContext();
	}

	BlockToBuildName = RowName;

	// Setting up preview
	if (PreviewedBlock.IsValid())
	{
		if (PreviewedBlock->GetClass() == BlockRow.BlockClass)
		{
			// No need to re-spawn the same block
			return;
		}
		PreviewedBlock->Destroy();
	}

	PreviewedBlock = GetWorld()->SpawnActor<ABlock>(BlockRow.BlockClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (PreviewedBlock.IsValid())
	{
		PreviewedBlock->SetActorHiddenInGame(true);
		// Tick function PreviewBlock() will set location, rotation and hiding
	}
}

void AEditorController::EnterBuildModeWithHovered()
{
	if (TrackGrid.IsValid() == false)
	{
		return;
	}
	
	if (bBuildMode == false)
	{
		bBuildMode = true;
		EnableBuildInputMappingContext();
	}

	PreviewedBlock = HoveredBlock;
	HoveredBlock = nullptr;

	BlockToBuildName = PreviewedBlock->GetBlockId();
}

void AEditorController::ExitBuildMode()
{
	bBuildMode = false;
	DisableBuildInputMappingContext();

	if (PreviewedBlock.IsValid() && PreviewedBlock->IsPendingKillPending() == false)
	{
		PreviewedBlock->Destroy();
	}

	CurrentActiveMenu = FEditorBuildMenu(EditorBuildMenuData);

	OnExitBuildMode.Broadcast();
}
