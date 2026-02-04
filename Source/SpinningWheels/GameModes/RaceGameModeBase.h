// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RaceGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ARaceGameModeBase : public AGameMode
{
	GENERATED_BODY()

private:

protected:

	TWeakObjectPtr<class AStartBlock> StartBlock;

	virtual void PrepareControllerForNewLap(AController* Controller);

public:

	//~ Begin AGameModeBase Interface
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	//~ End AGameModeBase Interface
	
};
