// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SpinningWheels/Core/Lap.h"
#include "SpinningWheels/Core/Simulation.h"
#include "RacePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerIdSetSignature, int32, PlayerId);

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

	bool bOnALap = false;

	// Sending simulation frames to the network
	FTimerHandle TimerServerSimulationFramesUpdate;

	void SendFramesToServer();
	int32 FramesSentToServer = 0;
	int32 NextFrameToSendToServer = 0;
	
	UFUNCTION(Server, Reliable)
	void ServerAddSimulationFrames(const TArray<FSimulationFrame>& ClientSimulationFrames);

	UFUNCTION(Server, Reliable)
	void ServerOnStartLap();

	UFUNCTION(Server, Reliable)
	void ServerOnFinishLap();

protected:

	virtual void OnNewBestLap(FRaceLap Lap);

public:

	//~ Begin APlayerState Interface
	virtual void OnRep_PlayerId() override;
	//~ End APlayerState Interface

	FOnPlayerIdSetSignature OnPlayerIdSet;
	
	void OnStartLap();
	void OnCheckpoint();
	void OnFinishLap();
	void OnCancelLap();

	void AddLap(FRaceLap NewLap);
	void ResetLaps();

	void AddSimulationFrame(const FSimulationFrame Frame);
	TOptional<FSimulationFrame> GetSimulationFrame(uint32 Index);
	
	bool IsOnALap() const { return bOnALap; }
	bool HasSimulationFrames() const { return SimulationFrames.Num() > 0; }
};
