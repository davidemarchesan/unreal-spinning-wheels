// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceGameState.generated.h"

/**
 *	Leaderboard row:
 *	- id
 *	- name
 *	- position ?
 *	- time (with sectors) ?
 *	- last time ?
 */

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ARaceGameState : public AGameState
{
	GENERATED_BODY()

private:

	TArray<FString> Leaderboard;

protected:

public:
	
};
