// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RaceGameMode.generated.h"

class ARaceController;

UCLASS()
class SPINNINGWHEELS_API ARaceGameMode : public AGameMode
{
	GENERATED_BODY()

private:

protected:

public:

	virtual void CancelLap(ARaceController* Controller);
};
