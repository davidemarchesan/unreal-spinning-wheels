// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGameState.h"

#include "Net/UnrealNetwork.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameModes/RaceGameMode.h"
#include "SpinningWheels/PlayerStates/TimeAttackPlayerState.h"

void ATimeAttackGameState::OnRep_Leaderboard()
{
	OnLeaderboardUpdate.Broadcast(Leaderboard);
}

void ATimeAttackGameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	Leaderboard.Reset();
}

void ATimeAttackGameState::HandleRaceMatchStateRacing()
{
	if (ARaceGameMode* GM = Cast<ARaceGameMode>(AuthorityGameMode))
	{
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
			}
		}
	}
}

void ATimeAttackGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATimeAttackGameState, Leaderboard);
}

void ATimeAttackGameState::OnNewBestLap(FRaceLap Lap)
{
	Leaderboard.AddPlayerNewBest(Lap);
	OnLeaderboardUpdate.Broadcast(Leaderboard);
}
