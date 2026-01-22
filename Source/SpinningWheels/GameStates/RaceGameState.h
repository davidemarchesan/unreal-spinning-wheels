// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceGameState.generated.h"

class ARacePlayerState;

/**
 *	Leaderboard row:
 *	- id
 *	- name
 *	- position ?
 *	- time (with sectors) ?
 *	- last time ?
 */

DECLARE_MULTICAST_DELEGATE(FOnLeaderboardUpdateSignature)

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ARaceGameState : public AGameState
{
	GENERATED_BODY()

private:

	UPROPERTY(ReplicatedUsing=OnRep_Leaderboard)
	TArray<float> Leaderboard;

	UFUNCTION()
	void OnRep_Leaderboard();

protected:

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnFinishLap(ARacePlayerState* PlayerState, float LapTime);

	FOnLeaderboardUpdateSignature OnLeaderboardUpdate;

	TArray<float> GetLeaderboard() const { return Leaderboard; };
	
};
