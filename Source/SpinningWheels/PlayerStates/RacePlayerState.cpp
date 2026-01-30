// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerState.h"

#include "Net/UnrealNetwork.h"
#include "SpinningWheels/GameStates/RaceGameState.h"

void ARacePlayerState::ServerAddSimulationFrames_Implementation(const TArray<FSimulationFrame>& ClientSimulationFrames)
{
	SimulationFrames.Append(ClientSimulationFrames);

	// UE_LOG(LogTemp, Warning, TEXT("(pid %d) SERVER - received me %d frames. now ive got %d"), GetPlayerId(), ClientSimulationFrames.Num(), SimulationFrames.Num());
}

void ARacePlayerState::ResetSimulationFrames()
{
	SimulationFrames.Empty();
}

void ARacePlayerState::SendFramesToServer()
{
	if (HasAuthority())
	{
		return;
	}

	if (bOnALap == false && NextFrameToSendToServer == SimulationFrames.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerServerSimulationFramesUpdate);
		NextFrameToSendToServer = 0;
		return;
	}

	TArray<FSimulationFrame> FramesToSend;
	for (int32 i = NextFrameToSendToServer; i < SimulationFrames.Num(); i++)
	{
		if (SimulationFrames.IsValidIndex(i))
		{
			FramesToSend.Add(SimulationFrames[i]);
		}
	}

	FramesSentToServer += FramesToSend.Num();

	ServerAddSimulationFrames(FramesToSend);
	NextFrameToSendToServer = SimulationFrames.Num();
}

void ARacePlayerState::ServerOnStartLap_Implementation()
{
	bOnALap = true;
}

void ARacePlayerState::ServerOnFinishLap_Implementation()
{
	bOnALap = false;
}

void ARacePlayerState::OnNewBestLap(FRaceLap Lap)
{
	// Child class
}

void ARacePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARacePlayerState, LastLap);
	DOREPLIFETIME(ARacePlayerState, BestLap);
}

void ARacePlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();
	OnPlayerIdSet.Broadcast(GetPlayerId());
}

void ARacePlayerState::OnStartLap()
{
	ResetCurrentLap();

	// Set timer to send server updates
	if (HasAuthority() == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerServerSimulationFramesUpdate);
		GetWorld()->GetTimerManager().SetTimer(TimerServerSimulationFramesUpdate, this,
		                                       &ARacePlayerState::SendFramesToServer,
		                                       SimulationConstants::NetworkUpdateFrequency, true);
	}

	bOnALap = true;
	CurrentLap = FRaceLap();
	
	if (HasAuthority() == false)
	{
		ServerOnStartLap();
	}

	OnCurrentLapUpdate.Broadcast(CurrentLap);
}

void ARacePlayerState::CancelLap()
{
	ResetCurrentLap();
	bOnALap = false;
}

void ARacePlayerState::CarOnCheckpoint(int32 CurrentFrameIndex)
{
	CurrentLap.AddSector(CurrentFrameIndex);
	OnCurrentLapUpdate.Broadcast(CurrentLap);
}

void ARacePlayerState::CarOnFinish(int32 CurrentFrameIndex)
{
	// UE_LOG(	LogTemp, Warning, TEXT("role %d, CarOnFinish"), GetLocalRole());
	CurrentLap.Close(CurrentFrameIndex);
	bOnALap = false;

	if (HasAuthority())
	{
		InternalAddLap(CurrentLap);
	}
	else
	{
		ServerAddLap(CurrentLap);
	}

	OnCurrentLapUpdate.Broadcast(CurrentLap);
}

void ARacePlayerState::ResetCurrentLap()
{
	CurrentLap = FRaceLap();
	ResetSimulationFrames();
}

void ARacePlayerState::InternalAddLap(FRaceLap NewLap)
{
	if (const UWorld* World = GetWorld())
	{
		if (ARaceGameState* RGS = World->GetGameState<ARaceGameState>())
		{
			NewLap.SetAt(RGS->GetServerWorldTimeSeconds());
			NewLap.SetPlayer(GetPlayerId(), GetPlayerName());

			LastLap = NewLap;
			if (LastLap < BestLap)
			{
				BestLap = LastLap;
				OnNewBestLap(BestLap);
			}
		}
	}
}

void ARacePlayerState::ServerAddLap_Implementation(FRaceLap NewLap)
{
	InternalAddLap(NewLap);
}

void ARacePlayerState::ResetLaps()
{
}

void ARacePlayerState::AddSimulationFrame(const FSimulationFrame Frame)
{
	if (bOnALap == false)
	{
		return;
	}

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
