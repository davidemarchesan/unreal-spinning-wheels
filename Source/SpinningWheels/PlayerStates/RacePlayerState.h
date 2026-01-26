// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SpinningWheels/Core/Lap.h"
#include "SpinningWheels/Core/Simulation.h"
#include "RacePlayerState.generated.h"

UCLASS()
class SPINNINGWHEELS_API ARacePlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	
	TArray<FRaceLap> Laps;
	FRaceLap CurrentLap;
	FRaceLap LastLap;
	FRaceLap BestLap;

	// UPROPERTY(Replicated) // todo: replicate using
	int8 LeaderboardPosition;

	float LapStartTime = 0.f;

	TArray<FSimulationFrame> SimulationFrames;

protected:

	virtual void OnNewBestLap(FRaceLap Lap);

public:
	void OnStartLap();
	void OnCheckpoint();
	void OnFinishLap();
	void OnCancelLap();

	void AddLap(FRaceLap NewLap);
	void ResetLaps();

	void AddSimulationFrame(const FSimulationFrame Frame);
	TOptional<FSimulationFrame> GetSimulationFrame(uint32 Index);
	bool IsRacing() const { return SimulationFrames.Num() > 0; } // todo: temp
};
