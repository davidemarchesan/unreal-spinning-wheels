// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/Match.h"
#include "RaceHUD.generated.h"

class SLeaderboardOverlay;

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

	TSharedPtr<SLeaderboardOverlay> LeaderboardOverlay;

protected:

	virtual void BeginPlay() override;

public:
	
};
