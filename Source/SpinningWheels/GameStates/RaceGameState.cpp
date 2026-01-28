// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameState.h"

#include "Net/UnrealNetwork.h"
#include "SpinningWheels/GameModes/RaceGameMode.h"

void ARaceGameState::OnRep_RaceMatchState()
{
	OnRaceMatchStateUpdate.Broadcast(RaceMatchState);

	switch (RaceMatchState)
	{
	case ERaceMatchState::RMS_WaitingForPlayers:
		HandleRaceMatchStateWaitingForPlayers();
		break;
	case ERaceMatchState::RMS_Racing:
		HandleRaceMatchStateRacing();
		break;
	case ERaceMatchState::RMS_Podium:
		HandleRaceMatchStatePodium();
		break;
	}
}

void ARaceGameState::HandleRaceMatchStateWaitingForPlayers()
{
}

void ARaceGameState::HandleRaceMatchStateRacing()
{
}

void ARaceGameState::HandleRaceMatchStatePodium()
{
}

void ARaceGameState::SetRaceMatchState(ERaceMatchState NewState)
{
	if (HasAuthority() == false)
	{
		return;
	}

	if (RaceMatchState == NewState)
	{
		return;
	}

	UE_LOG(LogGameState, Log, TEXT("Race Match State switched from %d to %d"), static_cast<uint8>(RaceMatchState),
	       static_cast<uint8>(NewState));

	RaceMatchState = NewState;

	// Server player only
	OnRep_RaceMatchState();
}

void ARaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARaceGameState, RaceMatchState);
}
