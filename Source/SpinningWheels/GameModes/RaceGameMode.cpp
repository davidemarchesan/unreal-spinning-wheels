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
	Super::PrepareControllerForNewLap(Controller);

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
		const FGuid NewGuid = FGuid::NewGuid();
		CurrentTrack = FTrack(NewGuid, "Track 1", 2); // todo: these are hard-coded test 

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

void ARaceGameMode::CancelLap(AController* Controller)
{
	if (APawn* ControlledPawn = Controller->GetPawn())
	{
		ControlledPawn->Destroy();
	}
	RestartPlayer(Controller);
}
