// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpinningWheels/Core/Match.h"
#include "SpinningWheels/Core/Track.h"
#include "RaceGameState.generated.h"

class ARacePlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRaceMatchStateUpdateSignature, ERaceMatchState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackUpdateSignature, const FTrack&, NewTrack);

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

	UPROPERTY(ReplicatedUsing=OnRep_CurrentTrack)
	FTrack CurrentTrack;

	UFUNCTION()
	virtual void OnRep_CurrentTrack();

public:

	//~ Begin AGameState Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End AGameState Interface

	void SetRaceMatchState(ERaceMatchState NewState);
	ERaceMatchState GetRaceMatchState() const { return RaceMatchState; }

	void SetCurrentTrack(const FTrack& NewTrack);
	FTrack GetCurrentTrack() const { return CurrentTrack; };
	
	/** Begin Delegates */
	FOnRaceMatchStateUpdateSignature OnRaceMatchStateUpdate;
	FOnTrackUpdateSignature OnTrackUpdate;
	/** End Delegates */
	
};
