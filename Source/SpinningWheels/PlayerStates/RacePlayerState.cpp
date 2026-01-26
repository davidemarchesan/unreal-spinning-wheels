// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerState.h"

void ARacePlayerState::ServerAddSimulationFrames_Implementation(const TArray<FSimulationFrame>& ClientSimulationFrames)
{
	SimulationFrames.Append(ClientSimulationFrames);

	UE_LOG(LogTemp, Warning, TEXT("pid %d Client sent me %d frames. now ive got %d"), GetPlayerId(), ClientSimulationFrames.Num(), SimulationFrames.Num());
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

	UE_LOG(LogTemp, Warning, TEXT("pid %d Sending %d Frames to Server"), GetPlayerId(), SimulationFrames.Num() - NextFrameToSendToServer);

	TArray<FSimulationFrame> FramesToSend;
	for (int32 i = NextFrameToSendToServer; i < SimulationFrames.Num(); i++)
	{
		if (SimulationFrames.IsValidIndex(i))
		{
			FramesToSend.Add(SimulationFrames[i]);
		}
	}

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

void ARacePlayerState::OnStartLap()
{
	SimulationFrames.Empty();

	if (HasAuthority() == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerServerSimulationFramesUpdate);
		GetWorld()->GetTimerManager().SetTimer(TimerServerSimulationFramesUpdate, this,
		                                       &ARacePlayerState::SendFramesToServer,
		                                       SimulationConstants::NetworkUpdateFrequency, true);
	}

	bOnALap = true;
	if (HasAuthority() == false)
	{
		ServerOnStartLap_Implementation();
	}
}

void ARacePlayerState::OnCheckpoint()
{
}

void ARacePlayerState::OnFinishLap()
{
	UE_LOG(LogTemp, Warning, TEXT("(role %d) ARacePlayerState::OnFinishLap with %d sim frames"), GetLocalRole(),
	       SimulationFrames.Num());

	bOnALap = false;
	if (HasAuthority() == false)
	{
		ServerOnFinishLap();
	}

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
	{
		// UE_LOG(LogTemp, Log, TEXT("ARacePlayerState::AddSimulationFrame Index(%d) %d %d %d %d"), SimulationFrames.Num(),
		//        Frame.DriveInputValue, Frame.BrakeInputValue, Frame.TurnInputValue, Frame.TurboInputValue);
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
