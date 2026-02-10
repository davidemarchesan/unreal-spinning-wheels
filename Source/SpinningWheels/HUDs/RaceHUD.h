// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/Leaderboard.h"
#include "SpinningWheels/Core/Match.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "RaceHUD.generated.h"

class ARaceGameState;
class ARacePlayerState;
class ARaceController;

UCLASS()
class SPINNINGWHEELS_API ARaceHUD : public AHUD
{
	GENERATED_BODY()

private:

	TWeakObjectPtr<ARaceGameState> RaceGameState;
	TWeakObjectPtr<ARacePlayerState> RacePlayerState;
	TWeakObjectPtr<ARaceController> RaceController;

	void InitializeRootOverlay();
	void InitializeOverlayLeaderboard();
	void InitializeOverlayServerMessages();
	void InitializeOverlayCountdown();
	void InitializeOverlayInfo();
	void InitializeOverlayLapTime();
	void InitializeOverlayMatchTime();

	void ShowModalOverlay(const TSharedPtr<SWidget>& Widget, const bool bFocus = true);
	void HideModalOverlay();

	/** Begin delegates bindings */
	void InitializeDelegates();
	void DeinitializeDelegates();
	
	UFUNCTION() void OnPlayerIdSet(int32 InPlayerId);
	UFUNCTION() void OnCurrentLapUpdate(const FRaceLap& CurrentLap);
	UFUNCTION() void OnLeaderboardUpdate(const FTimeAttackLeaderboard& Leaderboard);
	UFUNCTION() void OnRaceMatchStateUpdate(ERaceMatchState NewState);
	/** End delegates bindings */

	/** Begin Pointers to overlays */
	TSharedPtr<SOverlay> RootOverlay;
	TSharedPtr<SConstraintCanvas> RootCanvas;
	TSharedPtr<SOverlay> ModalOverlay;
	
	TSharedPtr<class SLeaderboardOverlay> LeaderboardOverlay;
	TSharedPtr<class SServerMessagesOverlay> ServerMessagesOverlay;
	TSharedPtr<class SCountdownOverlay> CountdownOverlay;
	TSharedPtr<class SInfoOverlay> InfoOverlay;
	TSharedPtr<class SLapTimeOverlay> LapTimeOverlay;
	TSharedPtr<class SMatchTimeOverlay> MatchTimeOverlay;
	/** End Pointers to overlays */

	int32 PlayerId;

	void HandleRaceMatchStateWaitingForPlayers();
	void HandleRaceMatchStateRacing();
	void HandleRaceMatchStatePodium();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	void ClearViewport();

	void InitLeaderboard();
	void ShowLeaderboard();
	void HideLeaderboard();

	void UpdateLeaderboard(const FTimeAttackLeaderboard& Leaderboard);

	void UpdateLapCountdown(int32 Seconds);

	void SetPlayerState(ARacePlayerState* InRacePlayerState);

	void SetMatchRemainingTime(const float Seconds);
	
};
