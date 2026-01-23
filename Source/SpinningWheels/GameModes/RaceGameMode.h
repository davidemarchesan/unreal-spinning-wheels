// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RaceGameMode.generated.h"

class ARaceController;

namespace RaceMatchState
{
	const FName WaitingForPlayers;
}

UCLASS()
class SPINNINGWHEELS_API ARaceGameMode : public AGameMode
{
	GENERATED_BODY()

private:

	FTimerHandle WaitingForPlayersTimer;

	void StartWaitingForPlayers();
	void StopWaitingForPlayers();

protected:

	//~ Begin AGameMode Interface
	virtual void HandleMatchHasStarted() override;
	//~ End AGameMode Interface

public:

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimeWaitingForPlayers;

	//~ Begin AGameMode Interface
	//~ End AGameMode Interface

	//~ Begin AGameModeBase Interface
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
	//~ End AGameModeBase Interface

	virtual void CancelLap(AController* Controller);
};
