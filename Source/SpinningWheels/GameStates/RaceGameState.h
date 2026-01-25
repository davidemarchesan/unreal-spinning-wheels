// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpinningWheels/Core/Match.h"
#include "RaceGameState.generated.h"

class ARacePlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRaceMatchStateUpdateSignature, ERaceMatchState, NewState);

UCLASS()
class SPINNINGWHEELS_API ARaceGameState : public AGameState
{
	GENERATED_BODY()

private:

protected:

	UPROPERTY(ReplicatedUsing=OnRep_RaceMatchState)
	ERaceMatchState RaceMatchState;

	UFUNCTION()
	virtual void OnRep_RaceMatchState();

	virtual void HandleRaceMatchStateWaitingForPlayers();
	virtual void HandleRaceMatchStateRacing();
	virtual void HandleRaceMatchStatePodium();

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetRaceMatchState(ERaceMatchState NewState);
	ERaceMatchState GetRaceMatchState() const { return RaceMatchState; }

	/** Begin Delegates */
	FOnRaceMatchStateUpdateSignature OnRaceMatchStateUpdate;
	/** End Delegates */
	
};
