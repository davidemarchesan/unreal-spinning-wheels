// Fill out your copyright notice in the Description page of Project Settings.



#include "TimeAttackGameState.h"

#include "Net/UnrealNetwork.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/PlayerStates/TimeAttackPlayerState.h"

void ATimeAttackGameState::OnRep_Leaderboard()
{
	UE_LOG(LogTemp, Warning, TEXT("Broadcasting leaderboard update"));
	OnLeaderboardUpdate.Broadcast();
}

void ATimeAttackGameState::HandleRaceMatchStateRacing()
{
	for (TObjectPtr<APlayerState> PS : PlayerArray)
	{
		if (PS.Get() == nullptr)
		{
			continue;
		}
		
		if (ARaceController* RC = Cast<ARaceController>(PS->GetPlayerController()))
		{
			RC->SetCanDrive(true);
		}
	}
}

void ATimeAttackGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATimeAttackGameState, Leaderboard);
}

void ATimeAttackGameState::OnNewBestLap(ATimeAttackPlayerState* PlayerState, FRaceLap Lap)
{
	UE_LOG(LogTemp, Warning, TEXT("ATimeAttackGameState leaderboard update"));
	Leaderboard.Add(FTimeAttackLeaderboardRow(PlayerState->GetPlayerId(), PlayerState->GetPlayerName(), Lap));
	OnLeaderboardUpdate.Broadcast();
}
