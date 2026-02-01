// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/Leaderboard.h"
#include "SpinningWheels/Core/Match.h"
#include "RaceHUD.generated.h"

class ARacePlayerState;

UCLASS()
class SPINNINGWHEELS_API ARaceHUD : public AHUD
{
	GENERATED_BODY()

private:

	void InitializeOverlays();
	void InitializeDelegates();

	/** Begin Pointers to overlays */
	TSharedPtr<class SLeaderboardOverlay> LeaderboardOverlay;
	TSharedPtr<class SServerMessagesOverlay> ServerMessagesOverlay;
	TSharedPtr<class SCountdownOverlay> CountdownOverlay;
	TSharedPtr<class SInfoOverlay> InfoOverlay;
	TSharedPtr<class SLapTimeOverlay> LapTimeOverlay;
	/** End Pointers to overlays */

	int32 PlayerId;

	UFUNCTION()
	void OnLeaderboardUpdate(const FTimeAttackLeaderboard& Leaderboard);

	UFUNCTION()
	void OnRaceMatchStateUpdate(ERaceMatchState NewState);

	void HandleRaceMatchStateWaitingForPlayers();
	void HandleRaceMatchStateRacing();
	void HandleRaceMatchStatePodium();

	UFUNCTION()
	void OnUpdateLapCountdown(int32 Seconds);

	UFUNCTION()
	void OnUpdateRacePlayerState(ARacePlayerState* RacePlayerState);

	UFUNCTION()
	void OnPlayerIdSet(int32 InPlayerId);

	UFUNCTION()
	void OnCurrentLapUpdate(const FRaceLap& CurrentLap);

protected:

	virtual void BeginPlay() override;

public:

	void ShowLeaderboard();
	void HideLeaderboard();
	
};
