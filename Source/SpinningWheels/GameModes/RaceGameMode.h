// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpinningWheels/Core/Match.h"
#include "RaceGameMode.generated.h"

class ARaceController;

UCLASS()
class SPINNINGWHEELS_API ARaceGameMode : public AGameMode
{
	GENERATED_BODY()

private:

	ERaceMatchState RaceMatchState;
	
	void StartWaitingForPlayers();

	TWeakObjectPtr<class AStartBlock> StartBlock;

protected:

	virtual void PrepareControllerForNewLap(AController* Controller);

	//~ Begin AGameMode Interface
	virtual void HandleMatchHasStarted() override;
	//~ End AGameMode Interface

public:

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimeWaitingForPlayers = 10.f;

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimeRacing = 300.f; // 5 minutes

	UPROPERTY(Category=Timers, EditDefaultsOnly)
	float TimePodium = 45.f;

	//~ Begin AGameMode Interface
	//~ End AGameMode Interface

	//~ Begin AGameModeBase Interface
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	//~ End AGameModeBase Interface

	virtual void CancelLap(AController* Controller);
};
