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

public:

	virtual void CancelLap(ARaceController* Controller) override;
	
};
