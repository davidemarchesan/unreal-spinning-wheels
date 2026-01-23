// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

	TSharedPtr<SLeaderboardOverlay> LeaderboardOverlay;

protected:

	virtual void BeginPlay() override;

public:
	
};
