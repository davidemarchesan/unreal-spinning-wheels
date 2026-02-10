// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameState.h"

#include "Net/UnrealNetwork.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"
#include "SpinningWheels/GameModes/RaceGameMode.h"

void ARaceGameState::OnRep_Leaderboard()
{
	OnLeaderboardUpdate.Broadcast(Leaderboard);
}

void ARaceGameState::OnRep_RaceMatchState()
{
	OnRaceMatchStateUpdate.Broadcast(RaceMatchState);

	switch (RaceMatchState)
	{
	case ERaceMatchState::RMS_LoadingGrid:
		HandleRaceMatchStateLoadingGrid();
		break;
	case ERaceMatchState::RMS_WaitingForPlayers:
		HandleRaceMatchStateWaitingForPlayers();
		break;
	case ERaceMatchState::RMS_Racing:
		HandleRaceMatchStateRacing();
		break;
	case ERaceMatchState::RMS_Podium:
		HandleRaceMatchStatePodium();
		break;
	default:
		break;
	}
}

void ARaceGameState::HandleRaceMatchStateLoadingGrid()
{
}

void ARaceGameState::HandleRaceMatchStateWaitingForPlayers()
{
}

void ARaceGameState::HandleRaceMatchStateRacing()
{
	if (const ARaceGameMode* GM = Cast<ARaceGameMode>(AuthorityGameMode))
	{
		ServerRacingEndTime = GetServerWorldTimeSeconds() + GM->TimeRacing;
		
		const float ServerStartDriveTime = GetServerWorldTimeSeconds() + GM->TimeStartDriveCountdown;
		for (TObjectPtr<APlayerState> PS : PlayerArray)
		{
			if (PS.Get() == nullptr)
			{
				continue;
			}

			if (ARaceController* RC = Cast<ARaceController>(PS->GetPlayerController()))
			{
				RC->PrepareForNewLap(ServerStartDriveTime);
				RC->SetRacingEndTime(ServerRacingEndTime);
			}
		}
	}
}

void ARaceGameState::HandleRaceMatchStatePodium()
{
}

void ARaceGameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	Leaderboard.Reset();
}

void ARaceGameState::SetRaceMatchState(ERaceMatchState NewState)
{
	if (HasAuthority() == false)
	{
		return;
	}

	if (RaceMatchState == NewState)
	{
		return;
	}

	RaceMatchState = NewState;

	// Server player only
	OnRep_RaceMatchState();
}

void ARaceGameState::SetCurrentTrack(const FTrack& NewTrack)
{
	CurrentTrack = NewTrack;

	// Server-player
	OnRep_CurrentTrack();
}

void ARaceGameState::OnNewBestLap(FRaceLap Lap)
{
	if (AcceptsNewLaps() == false)
	{
		return;
	}
	
	Leaderboard.AddPlayerNewBest(Lap);
	OnLeaderboardUpdate.Broadcast(Leaderboard);
}

void ARaceGameState::OnRep_CurrentTrack()
{
	OnTrackUpdate.Broadcast(CurrentTrack);
}

void ARaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARaceGameState, RaceMatchState);
	DOREPLIFETIME(ARaceGameState, Leaderboard);
	DOREPLIFETIME(ARaceGameState, ServerRacingEndTime);
}
