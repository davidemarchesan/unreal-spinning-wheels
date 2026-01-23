// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerState.h"

#include "SpinningWheels/GameStates/RaceGameState.h"

void ARacePlayerState::OnStartLap()
{
	LapStartTime = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("StartLap %f"), LapStartTime);

	CurrentLap = FRaceLap(GetWorld()->GetTimeSeconds());
}

void ARacePlayerState::OnCheckpoint()
{
	float SectorTime = GetWorld()->GetTimeSeconds() - LapStartTime;
	// UE_LOG(LogTemp, Warning, TEXT("Checkpoint %f (role %d)"), SectorTime, GetLocalRole());

	CurrentLap.Sector(GetWorld()->GetTimeSeconds());
}

void ARacePlayerState::OnFinishLap()
{
	float Now = GetWorld()->GetTimeSeconds();
	float LapTime = Now - LapStartTime;

	// UE_LOG(LogTemp, Warning, TEXT("FinishLap %f (role %d)"), LapTime, GetLocalRole());

	CurrentLap.Close(GetWorld()->GetTimeSeconds());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentLap.ToString());

	// todo: server only: should notify gamestate to check and update leaderboard
	if (HasAuthority())
	{
		if (UWorld* World = GetWorld())
		{
			if (ARaceGameState* GS = Cast<ARaceGameState>(World->GetGameState()))
			{
				GS->OnFinishLap(this, LapTime);
			}
		}
	}
	
}

void ARacePlayerState::OnCancelLap()
{
}

void ARacePlayerState::AddLap(FRaceLap NewLap)
{
}

void ARacePlayerState::ResetLaps()
{
}
