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

	//~ Begin Lap management
	TArray<FRaceLap> Laps;
	FRaceLap CurrentLap;

	UPROPERTY(Replicated)
	FRaceLap LastLap;

	UPROPERTY(Replicated)
	FRaceLap BestLap;

	void ResetCurrentLap();
	
	UFUNCTION(Server, Reliable)
	void ServerAddLap(FRaceLap NewLap);

	void InternalAddLap(FRaceLap NewLap);
	//~ End Lap management

	// UPROPERTY(Replicated) // todo: replicate using
	int8 LeaderboardPosition;

	float LapStartTime = 0.f;

	//~ Begin Simulation Frames management
	TArray<FSimulationFrame> SimulationFrames;
	
	void ResetSimulationFrames();
	//~ End Simulation Frames management
	
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerId() override;
	//~ End APlayerState Interface

	FOnPlayerIdSetSignature OnPlayerIdSet;
	
	void OnStartLap();
	void CancelLap();

	void CarOnCheckpoint(int32 CurrentFrameIndex);
	void CarOnFinish(int32 CurrentFrameIndex);
	
	void ResetLaps();

	void AddSimulationFrame(const FSimulationFrame Frame);
	TOptional<FSimulationFrame> GetSimulationFrame(uint32 Index);
	
	bool IsOnALap() const { return bOnALap; }
	bool HasSimulationFrames() const { return SimulationFrames.Num() > 0; }
};
