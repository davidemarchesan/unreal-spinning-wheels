// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameState.h"

#include "Net/UnrealNetwork.h"

void ARaceGameState::OnRep_Leaderboard()
{
	UE_LOG(LogTemp, Warning, TEXT("ARaceGameState::leaderboard has changed!  (role %d)"), GetLocalRole());
	OnLeaderboardUpdate.Broadcast();
}

void ARaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARaceGameState, Leaderboard);
}

void ARaceGameState::OnFinishLap(ARacePlayerState* PlayerState, float LapTime)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ARaceGameState::OnFinishLap received %f"), LapTime);
		Leaderboard.Add(LapTime);
	}
}
