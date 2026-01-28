// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGameState.h"

#include "Net/UnrealNetwork.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameModes/RaceGameMode.h"
#include "SpinningWheels/PlayerStates/TimeAttackPlayerState.h"

void ATimeAttackGameState::OnRep_Leaderboard()
{
	UE_LOG(LogTemp, Warning, TEXT("im on client, and leaderboard has been modified"));
	OnLeaderboardUpdate.Broadcast();
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
	// Leaderboard.Add(FTimeAttackLeaderboardRow(PlayerState->GetPlayerId(), PlayerState->GetPlayerName(), Lap));
	Leaderboard.AddPlayerNewBest(Lap);
	OnLeaderboardUpdate.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("Leaderboard update %s"), *Leaderboard.ToString());
}
