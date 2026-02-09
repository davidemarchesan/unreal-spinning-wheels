// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceGameModeBase.h"
#include "SpinningWheels/Core/Track.h"
#include "SpinningWheels/Core/Match.h"
#include "RaceGameMode.generated.h"

class ARaceController;
class ATrackGrid;

UCLASS()
class SPINNINGWHEELS_API ARaceGameMode : public ARaceGameModeBase
{
	GENERATED_BODY()

private:
	void InitializeGrid();

	FTimerHandle WaitingForPlayersTimer;
	void StartWaitingForPlayers();
	void StopWaitingForPlayers();

protected:
	void SetRaceMatchState(ERaceMatchState NewState);
	ERaceMatchState RaceMatchState;

	virtual void OnNewRaceMatchState();
	virtual void HandleRaceMatchStateLoadingGrid();
	virtual void HandleRaceMatchStateWaitingForPlayers();
	virtual void HandleRaceMatchStateRacing();
	virtual void HandleRaceMatchStatePodium();

	virtual void PrepareControllerForNewLap(AController* Controller) override;

	FTrack CurrentTrack;

	//~ Begin AGameMode Interface
	virtual void OnMatchStateSet() override;
	virtual void HandleMatchHasStarted() override;
	//~ End AGameMode Interface

public:
	ERaceMatchState GetRaceMatchState() const { return RaceMatchState; }

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimeWaitingForPlayers = 10.f;

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimeRacing = 300.f; // 5 minutes

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimePodium = 45.f;

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimeStartDriveCountdown = 4;

	FTrack GetCurrentTrack() { return CurrentTrack; }

	virtual void CancelLap(AController* Controller) override;

	void ControllerIsReady(AController* Controller);
};
