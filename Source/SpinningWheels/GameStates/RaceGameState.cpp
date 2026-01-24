// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameState.h"

#include "Net/UnrealNetwork.h"

void ARaceGameState::OnRep_RaceMatchState()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_RaceMatchState -> broadcasting"));
	OnRaceMatchStateUpdate.Broadcast(RaceMatchState);
}

void ARaceGameState::SetRaceMatchState(ERaceMatchState NewState)
{
	if (HasAuthority() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("SetRaceMatchState no auth"));
		return;
	}
	
	if (RaceMatchState == NewState)
	{
		UE_LOG(LogTemp, Error, TEXT("SetRaceMatchState new state is not new"));
		UE_LOG(LogTemp, Warning, TEXT("SetRaceMatchState %d -> %d"), static_cast<uint8>(RaceMatchState), static_cast<uint8>(NewState));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("SetRaceMatchState %d -> %d"), static_cast<uint8>(RaceMatchState), static_cast<uint8>(NewState));
	RaceMatchState = NewState;
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

	UE_LOG(LogTemp, Warning, TEXT("ARaceGameState: server wants to wait for players %f"), Seconds);
	
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
	GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimer, this, &ARaceGameState::StopWaitingForPlayers, Seconds, false);

	SetRaceMatchState(ERaceMatchState::RMS_WaitingForPlayers);
	
}

void ARaceGameState::StopWaitingForPlayers()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
	UE_LOG(LogTemp, Warning, TEXT("ARaceGameState: enough waitin! let's race"));
	SetRaceMatchState(ERaceMatchState::RMS_Racing);
}
