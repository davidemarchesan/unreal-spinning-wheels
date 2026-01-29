// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceGameState.h"
#include "SpinningWheels/Core/Leaderboard.h"
#include "TimeAttackGameState.generated.h"

class ATimeAttackPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeaderboardUpdateSignature, const FTimeAttackLeaderboard&, Leaderboard);

UCLASS()
class SPINNINGWHEELS_API ATimeAttackGameState : public ARaceGameState
{
	GENERATED_BODY()

private:

	UPROPERTY(ReplicatedUsing=OnRep_Leaderboard)
	FTimeAttackLeaderboard Leaderboard;

	UFUNCTION()
	void OnRep_Leaderboard();

protected:

	//~ Begin AGameState Interface
	virtual void HandleMatchIsWaitingToStart() override;
	//~ End AGameState Interface

	//~ Begin ARaceGameState Interface
	virtual void HandleRaceMatchStateRacing() override;
	//~ End ARaceGameState Interface
	
public:

	const FTimeAttackLeaderboard& GetLeaderboard() const { return Leaderboard; }

	//~ Begin AGameState Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End AGameState Interface

	void OnNewBestLap(FRaceLap Lap);
	
	FOnLeaderboardUpdateSignature OnLeaderboardUpdate;
};
