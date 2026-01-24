// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"
#include "SpinningWheels/Actors/Blocks/StartBlock.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameStates/RaceGameState.h"

void ARaceGameMode::StartWaitingForPlayers()
{
	if (ARaceGameState* GS = GetWorld()->GetGameState<ARaceGameState>())
	{
		GS->StartWaitingForPlayers(TimeWaitingForPlayers);
	}
}

void ARaceGameMode::PrepareControllerForNewLap(AController* Controller)
{
	if (RaceMatchState == ERaceMatchState::RMS_WaitingForPlayers)
	{
		return;
	}
}

void ARaceGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	StartWaitingForPlayers();
}

void ARaceGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (ARaceController* RC = Cast<ARaceController>(NewPlayer))
	{
		RC->SetCanDrive(false);
	}
	
	if (StartBlock.IsValid() == false)
	{
		if (UWorld* World = GetWorld())
		{
			for (TActorIterator<AStartBlock> It(World); It; ++It)
			{
				AStartBlock* Start = *It;
				if (Start)
				{
					StartBlock = Start;
					break;
				}
			}
		}
	}

	if (StartBlock.IsValid() == false)
	{
		// We did not find a start block :(
		return;
	}

	FVector OutLocation;
	FRotator OutRotation;
	FTransform SpawnTransform;

	if (StartBlock.IsValid())
	{
		StartBlock->GetStartingPoint(OutLocation, OutRotation);

		SpawnTransform.SetLocation(OutLocation);
		SpawnTransform.SetRotation(OutRotation.Quaternion());
	}

	RestartPlayerAtTransform(NewPlayer, SpawnTransform);
}

void ARaceGameMode::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtTransform %s"),
	       (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtTransform: Tried to restart a spectator-only player!"));
		return;
	}

	FRotator SpawnRotation = SpawnTransform.GetRotation().Rotator();

	if (NewPlayer->GetPawn() != nullptr)
	{
		NewPlayer->GetPawn()->SetActorLocation(SpawnTransform.GetLocation());
		NewPlayer->GetPawn()->SetActorRotation(SpawnTransform.GetRotation());
	}
	else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);
		}
	}

	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		FinishRestartPlayer(NewPlayer, SpawnRotation);
	}

	PrepareControllerForNewLap(NewPlayer);
}

void ARaceGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	NewPlayer->Possess(NewPlayer->GetPawn());

	// If the Pawn is destroyed as part of possession we have to abort
	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		SetPlayerDefaults(NewPlayer->GetPawn());
		K2_OnRestartPlayer(NewPlayer);
	}
}

void ARaceGameMode::CancelLap(AController* Controller)
{
	RestartPlayer(Controller);
}
