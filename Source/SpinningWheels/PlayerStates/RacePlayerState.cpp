// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerState.h"

#include "SpinningWheels/GameStates/RaceGameState.h"

void ARacePlayerState::OnNewBestLap(FRaceLap Lap)
{
}

void ARacePlayerState::OnStartLap()
{
	CurrentLap = FRaceLap(GetWorld()->GetTimeSeconds());
	SimulationFrames.Empty();
}

void ARacePlayerState::OnCheckpoint()
{
	CurrentLap.AddSector(GetWorld()->GetTimeSeconds());
}

void ARacePlayerState::OnFinishLap()
{
	
	CurrentLap.Close(GetWorld()->GetTimeSeconds());

	

	// todo: server only: should notify gamestate to check and update leaderboard
	if (HasAuthority())
	{

		LastLap = CurrentLap;
		if (LastLap < BestLap)
		{
			BestLap = LastLap;
			OnNewBestLap(BestLap);
		}
	}
	
}

void ARacePlayerState::OnCancelLap()
{
	if (HasAuthority())
	{
		CurrentLap = FRaceLap();
	}
}

void ARacePlayerState::AddLap(FRaceLap NewLap)
{
}

void ARacePlayerState::ResetLaps()
{
}

void ARacePlayerState::AddSimulationFrame(const FSimulationFrame Frame)
{
	if (GetLocalRole() != ROLE_Authority)
		UE_LOG(LogTemp, Log, TEXT("ARacePlayerState::AddSimulationFrame Index(%d) %d %d %d %d"), SimulationFrames.Num(), Frame.DriveInputValue, Frame.BrakeInputValue, Frame.TurnInputValue, Frame.TurboInputValue);
		
	SimulationFrames.Add(Frame);
}

TOptional<FSimulationFrame> ARacePlayerState::GetSimulationFrame(uint32 Index)
{
	if (SimulationFrames.IsValidIndex(Index))
	{
		return SimulationFrames[Index];
	}
	else
	{
		return TOptional<FSimulationFrame>();
	}
}
