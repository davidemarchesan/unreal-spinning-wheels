// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceGameState.h"
#include "SpinningWheels/Core/Leaderboard.h"
#include "TimeAttackGameState.generated.h"

class ATimeAttackPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeaderboardUpdateSignature);

UCLASS()
class SPINNINGWHEELS_API ATimeAttackGameState : public ARaceGameState
{
	GENERATED_BODY()

private:

	UPROPERTY(ReplicatedUsing=OnRep_Leaderboard)
	TArray<FTimeAttackLeaderboardRow> Leaderboard;

	UFUNCTION()
	void OnRep_Leaderboard();

protected:

	//~ Begin ARaceGameState Interface
	virtual void HandleRaceMatchStateRacing() override;
	//~ End ARaceGameState Interface
	
public:

	FOnLeaderboardUpdateSignature OnLeaderboardUpdate;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnNewBestLap(ATimeAttackPlayerState* PlayerState, FRaceLap Lap);

	TArray<FTimeAttackLeaderboardRow> GetLeaderboard() const { return Leaderboard; }
};
