// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameState.h"

#include "Net/UnrealNetwork.h"

void ARaceGameState::OnRep_RaceMatchState()
{
	OnRaceMatchStateUpdate.Broadcast(RaceMatchState);
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

	RaceMatchState = NewState;

	// Server player only
	OnRaceMatchStateUpdate.Broadcast(RaceMatchState);
}

void ARaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARaceGameState, RaceMatchState);
}

void ARaceGameState::StartWaitingForPlayers(float Seconds)
{
	if (HasAuthority() == false)
	{
		return;
	}
	
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
	GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimer, this, &ARaceGameState::StopWaitingForPlayers, Seconds, false);

	SetRaceMatchState(ERaceMatchState::RMS_WaitingForPlayers);
	
}

void ARaceGameState::StopWaitingForPlayers()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
	SetRaceMatchState(ERaceMatchState::RMS_Racing);
}
