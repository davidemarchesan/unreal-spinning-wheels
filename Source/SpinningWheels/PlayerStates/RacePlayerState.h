// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SpinningWheels/Core/Lap.h"
#include "RacePlayerState.generated.h"

UCLASS()
class SPINNINGWHEELS_API ARacePlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	
	TArray<FRaceLap> Laps;
	FRaceLap CurrentLap;

	// UPROPERTY(Replicated) // todo: replicate using
	int8 LeaderboardPosition;

	float LapStartTime = 0.f;

protected:

public:
	void OnStartLap();
	void OnCheckpoint();
	void OnFinishLap();
	void OnCancelLap();

	void AddLap(FRaceLap NewLap);
	void ResetLaps();
};
