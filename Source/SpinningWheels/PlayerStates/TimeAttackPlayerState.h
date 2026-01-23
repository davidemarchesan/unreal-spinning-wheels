// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RacePlayerState.h"
#include "TimeAttackPlayerState.generated.h"

class ATimeAttackGameState;

UCLASS()
class SPINNINGWHEELS_API ATimeAttackPlayerState : public ARacePlayerState
{
	GENERATED_BODY()

private:

	ATimeAttackGameState* GetTimeAttackGameState();

protected:

	virtual void OnNewBestLap(FRaceLap Lap) override;

public:
	
};
