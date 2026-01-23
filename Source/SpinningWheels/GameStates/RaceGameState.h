// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceGameState.generated.h"

class ARacePlayerState;

UCLASS()
class SPINNINGWHEELS_API ARaceGameState : public AGameState
{
	GENERATED_BODY()

private:

protected:

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
