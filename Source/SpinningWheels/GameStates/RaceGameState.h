// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpinningWheels/Core/Leaderboard.h"
#include "SpinningWheels/Core/Match.h"
#include "SpinningWheels/Core/Track.h"
#include "RaceGameState.generated.h"

class ARacePlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRaceMatchStateUpdateSignature, ERaceMatchState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackUpdateSignature, const FTrack&, NewTrack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeaderboardUpdateSignature, TArray<FRaceLap>, Leaderboard, TArray<int32>, BestSectors);

UCLASS()
class SPINNINGWHEELS_API ARaceGameState : public AGameState
{
	GENERATED_BODY()

private:

	// UPROPERTY(ReplicatedUsing=OnRep_Leaderboard)
	// FTimeAttackLeaderboard Leaderboard;
	
	// UPROPERTY(ReplicatedUsing=OnRep_Leaderboard)
	UPROPERTY(Replicated)
	TArray<FRaceLap> Leaderboard;

	// UFUNCTION()
	// void OnRep_Leaderboard();

	TArray<FRaceLap> TempLeaderboard;

	UPROPERTY(Replicated)
	TArray<int32> BestSectors;

	void AddPlayerNewBest(FRaceLap NewLap);
	void CalcBestSectors();

	UPROPERTY(Replicated);
	float ServerRacingEndTime = 0.f;

	UFUNCTION(Client, Reliable)
	void ClientUpdateLeaderboard(const TArray<FRaceLap>& InLeaderboard, const TArray<int32>& InBestSectors);

protected:

	UPROPERTY(ReplicatedUsing=OnRep_RaceMatchState)
	ERaceMatchState RaceMatchState;

	UFUNCTION()
	virtual void OnRep_RaceMatchState();

	virtual void HandleRaceMatchStateLoadingGrid();
	virtual void HandleRaceMatchStateWaitingForPlayers();
	virtual void HandleRaceMatchStateRacing();
	virtual void HandleRaceMatchStatePodium();

	//~ Begin AGameState Interface
	virtual void HandleMatchIsWaitingToStart() override;
	//~ End AGameState Interface

	UPROPERTY(ReplicatedUsing=OnRep_CurrentTrack)
	FTrack CurrentTrack;

	UFUNCTION()
	virtual void OnRep_CurrentTrack();

public:

	//~ Begin AGameState Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End AGameState Interface

	void SetRaceMatchState(ERaceMatchState NewState);
	ERaceMatchState GetRaceMatchState() const { return RaceMatchState; }

	float GetServerRacingEndTime() const { return ServerRacingEndTime; }

	void SetCurrentTrack(const FTrack& NewTrack);
	FTrack GetCurrentTrack() const { return CurrentTrack; };

	bool AcceptsNewLaps() const { return RaceMatchState == ERaceMatchState::RMS_Racing; }
	
	// const FTimeAttackLeaderboard& GetLeaderboard();
	void GetLeaderboard(TArray<FRaceLap>& OutLeaderboard, TArray<int32>& OutBestSectors);
	void OnNewBestLap(FRaceLap Lap);
	
	/** Begin Delegates */
	FOnRaceMatchStateUpdateSignature OnRaceMatchStateUpdate;
	FOnTrackUpdateSignature OnTrackUpdate;
	FOnLeaderboardUpdateSignature OnLeaderboardUpdate;
	/** End Delegates */
	
};
