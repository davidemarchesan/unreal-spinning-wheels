// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RacePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ARacePlayerState : public APlayerState
{
	GENERATED_BODY()

private:

	int8 LeaderboardPosition;

	// Current lap timing

protected:

public:
	
};
