// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerState.h"

#include "SpinningWheels/GameStates/RaceGameState.h"

void ARacePlayerState::OnNewBestLap(FRaceLap Lap)
{
}

void ARacePlayerState::OnStartLap()
{
	CurrentLap = FRaceLap(GetWorld()->GetTimeSeconds());
}

void ARacePlayerState::OnCheckpoint()
{
	CurrentLap.AddSector(GetWorld()->GetTimeSeconds());
}

void ARacePlayerState::OnFinishLap()
{
	
	CurrentLap.Close(GetWorld()->GetTimeSeconds());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentLap.ToString());

	// todo: server only: should notify gamestate to check and update leaderboard
	if (HasAuthority())
	{

		LastLap = CurrentLap;
		if (LastLap < BestLap)
		{
			BestLap = LastLap;
			OnNewBestLap(BestLap);
		}
	}
	
}

void ARacePlayerState::OnCancelLap()
{
	if (HasAuthority())
	{
		CurrentLap = FRaceLap();
	}
}

void ARacePlayerState::AddLap(FRaceLap NewLap)
{
}

void ARacePlayerState::ResetLaps()
{
}
