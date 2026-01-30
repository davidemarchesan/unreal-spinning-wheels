// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"
#include "SpinningWheels/Actors/Blocks/StartBlock.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameStates/RaceGameState.h"

void ARaceGameMode::StartWaitingForPlayers()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
	GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimer, this, &ARaceGameMode::StopWaitingForPlayers,
	                                       TimeWaitingForPlayers, false);

	SetRaceMatchState(ERaceMatchState::RMS_WaitingForPlayers);
}

void ARaceGameMode::StopWaitingForPlayers()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);

	SetRaceMatchState(ERaceMatchState::RMS_Racing);
}

void ARaceGameMode::OnNewRaceMatchState()
{
	switch (RaceMatchState)
	{
	case ERaceMatchState::RMS_WaitingForPlayers:
		HandleRaceMatchStateWaitingForPlayers();
		break;
	case ERaceMatchState::RMS_Racing:
		HandleRaceMatchStateRacing();
		break;
	case ERaceMatchState::RMS_Podium:
		HandleRaceMatchStatePodium();
		break;
	}
}

void ARaceGameMode::HandleRaceMatchStateWaitingForPlayers()
{
}

void ARaceGameMode::HandleRaceMatchStateRacing()
{
}

void ARaceGameMode::HandleRaceMatchStatePodium()
{
}

void ARaceGameMode::PrepareControllerForNewLap(AController* Controller)
{
	if (RaceMatchState == ERaceMatchState::RMS_WaitingForPlayers)
	{
		return;
	}
}

void ARaceGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	if (MatchState == MatchState::EnteringMap)
	{
		CurrentTrack = FTrack("qwer-tyui-opas", "Track 1", 2); // todo: these are hard-coded test 

		if (ARaceGameState* RGS = GetGameState<ARaceGameState>())
		{
			RGS->SetCurrentTrack(CurrentTrack);
		}
	}
}

void ARaceGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	StartWaitingForPlayers();
}

void ARaceGameMode::SetRaceMatchState(ERaceMatchState NewState)
{
	if (RaceMatchState == NewState)
	{
		return;
	}
	
	RaceMatchState = NewState;

	if (ARaceGameState* GS = Cast<ARaceGameState>(GetWorld()->GetGameState()))
	{
		GS->SetRaceMatchState(RaceMatchState);
	}
}

void ARaceGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (ARaceController* RC = Cast<ARaceController>(NewPlayer))
	{
		RC->SetPhase(ERaceControllerPhase::RCP_Respawning);
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
		UE_LOG(LogGameMode, Error, TEXT("No start block has been found"));
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
	if (APawn* ControlledPawn = Controller->GetPawn())
	{
		ControlledPawn->Destroy();
	}
	RestartPlayer(Controller);
}
