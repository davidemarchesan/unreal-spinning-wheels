// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceGameMode.h"
#include "TimeAttackGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ATimeAttackGameMode : public ARaceGameMode
{
	GENERATED_BODY()

private:

protected:

	//~ Begin ARaceGameMode Interface
	virtual void PrepareControllerForNewLap(AController* Controller) override;
	//~ End AGameMode Interface

public:

	virtual void CancelLap(AController* Controller) override;
	
};
