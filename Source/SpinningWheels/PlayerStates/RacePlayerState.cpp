// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerState.h"

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

	// if (HasSimulationFrames() == false)
	// {
	// 	return;
	// }

	TArray<FSimulationFrame> FramesToSend;
	for (int32 i = NextFrameToSendToServer; i < SimulationFrames.Num(); i++)
	{
		if (SimulationFrames.IsValidIndex(i))
		{
			FramesToSend.Add(SimulationFrames[i]);
		}
	}

	FramesSentToServer += FramesToSend.Num();
	// UE_LOG(LogTemp, Warning, TEXT("(pid %d) CLIENT - Sending %d Frames to Server | total: %d"), GetPlayerId(), SimulationFrames.Num() - NextFrameToSendToServer, FramesSentToServer);
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
}

void ARacePlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();

	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerId() %d"), GetPlayerId());
	OnPlayerIdSet.Broadcast(GetPlayerId());
}

void ARacePlayerState::OnStartLap()
{
	ResetSimulationFrames();

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
}

void ARacePlayerState::OnCancelLap()
{
	if (HasAuthority())
	{
		CurrentLap = FRaceLap();
	}
}

void ARacePlayerState::CarOnCheckpoint(int32 CurrentFrameIndex)
{
	CurrentLap.AddSector(CurrentFrameIndex);
}

void ARacePlayerState::CarOnFinish(int32 CurrentFrameIndex)
{
	CurrentLap.Close(CurrentFrameIndex);
	bOnALap = false;

	UE_LOG(LogTemp, Warning, TEXT("LAP %s"), *CurrentLap.ToString());
	// if (HasAuthority() == false)
	// {
	// 	ServerOnFinishLap();
	// }

	// CurrentLap.Close(GetWorld()->GetTimeSeconds());
	//
	//
	// // todo: server only: should notify gamestate to check and update leaderboard
	// if (HasAuthority())
	// {
	// 	LastLap = CurrentLap;
	// 	if (LastLap < BestLap)
	// 	{
	// 		BestLap = LastLap;
	// 		OnNewBestLap(BestLap);
	// 	}
	// }
}

void ARacePlayerState::AddLap(FRaceLap NewLap)
{
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
