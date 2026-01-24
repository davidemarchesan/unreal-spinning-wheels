// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/Match.h"
#include "RaceHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ARaceHUD : public AHUD
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnLeaderboardUpdate();

	UFUNCTION()
	void OnRaceMatchStateUpdate(ERaceMatchState NewState);

	void HandleRaceMatchStateWaitingForPlayers();
	void HandleRaceMatchStateRacing();
	void HandleRaceMatchStatePodium();

protected:

	virtual void BeginPlay() override;

public:

private:

	/** Pointers to overlays */
	TSharedPtr<class SLeaderboardOverlay> LeaderboardOverlay;
	TSharedPtr<class SServerMessagesOverlay> ServerMessagesOverlay;
	
};
