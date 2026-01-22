// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerState.h"

#include "SpinningWheels/GameStates/RaceGameState.h"

void ARacePlayerState::OnStartLap()
{
	LapStartTime = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("StartLap %f"), LapStartTime);
}

void ARacePlayerState::OnCheckpoint()
{
	float SectorTime = GetWorld()->GetTimeSeconds() - LapStartTime;
	UE_LOG(LogTemp, Warning, TEXT("Checkpoint %f (role %d)"), SectorTime, GetLocalRole());
}

void ARacePlayerState::OnFinishLap()
{
	float Now = GetWorld()->GetTimeSeconds();
	float LapTime = Now - LapStartTime;

	UE_LOG(LogTemp, Warning, TEXT("FinishLap %f (role %d)"), LapTime, GetLocalRole());

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
