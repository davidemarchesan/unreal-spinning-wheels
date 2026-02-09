// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "SpinningWheels/Actors/TrackGrid.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"
#include "SpinningWheels/Subsystems/GameInstance/RaceServerSubsystem.h"
#include "SpinningWheels/Subsystems/GameInstance/TracksSubsystem.h"

void ARaceGameMode::InitializeGrid()
{
	TrackGrid = GetWorld()->SpawnActor<ATrackGrid>(DefaultTrackGridClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (TrackGrid.IsValid())
	{
		if (const UGameInstance* GameInstance = GetGameInstance())
		{
			if (URaceServerSubsystem* RaceServerSubsystem = GameInstance->GetSubsystem<URaceServerSubsystem>())
			{
				// TEST ONLY
				if (RaceServerSubsystem->GetCurrentTrackIndex() == INDEX_NONE)
				{
					if (const UTracksSubsystem* TracksSubsystem = GameInstance->GetSubsystem<UTracksSubsystem>())
					{
						RaceServerSubsystem->SetTracksPlaylist(TracksSubsystem->GetTracks());
					}
				}
				// END TEST ONLY

				const FTrackSaveData TrackToLoadSaveData = RaceServerSubsystem->GetCurrentTrack();
				CurrentTrack = FTrack(TrackToLoadSaveData.Id, TrackToLoadSaveData.Name);

				TrackGrid->Initialize(TrackConstants::Size, TrackConstants::Size, TrackToLoadSaveData);
			}
		}
	}

	SetRaceMatchState(ERaceMatchState::RMS_WaitingForPlayers);
}

void ARaceGameMode::StartWaitingForPlayers()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimerHandle, this, &ARaceGameMode::StopWaitingForPlayers,
	                                       TimeWaitingForPlayers, false);
}

void ARaceGameMode::StopWaitingForPlayers()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimerHandle);
	SetRaceMatchState(ERaceMatchState::RMS_Racing);
}

void ARaceGameMode::StartRacingTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("RACING TIMER START"));
	GetWorld()->GetTimerManager().ClearTimer(RacingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RacingTimerHandle, this, &ARaceGameMode::EndRacingTimer,
	                                       TimeRacing, false);
}

void ARaceGameMode::EndRacingTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("RACING IS OVER"));
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimerHandle);
	SetRaceMatchState(ERaceMatchState::RMS_Podium);
}

void ARaceGameMode::StartPodiumTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("PODIUM TIMER START"));
	GetWorld()->GetTimerManager().ClearTimer(PodiumTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(PodiumTimerHandle, this, &ARaceGameMode::EndPodiumTimer,
	                                       TimePodium, false);
}

void ARaceGameMode::EndPodiumTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("PODIUM IS OVER"));
	GetWorld()->GetTimerManager().ClearTimer(PodiumTimerHandle);

	// Prepare the server to go to next track
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (URaceServerSubsystem* RaceServerSubsystem = GameInstance->GetSubsystem<URaceServerSubsystem>())
		{
			RaceServerSubsystem->GoToNextTrack(); 
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("travel to next map, wish me luck"));

	if (UWorld* World = GetWorld())
	{
		bUseSeamlessTravel = true;
		World->ServerTravel("L_Race", true);
	}

	// Disable players input
	// Load next map
	// Server travel
	
	
}

void ARaceGameMode::OnRaceMatchStateSet()
{
	switch (RaceMatchState)
	{
	case ERaceMatchState::RMS_LoadingGrid:
		HandleRaceMatchStateLoadingGrid();
		break;
	case ERaceMatchState::RMS_WaitingForPlayers:
		HandleRaceMatchStateWaitingForPlayers();
		break;
	case ERaceMatchState::RMS_Racing:
		HandleRaceMatchStateRacing();
		break;
	case ERaceMatchState::RMS_Podium:
		HandleRaceMatchStatePodium();
		break;
	default:
		break;
	}
}

void ARaceGameMode::HandleRaceMatchStateLoadingGrid()
{
	InitializeGrid();
}

void ARaceGameMode::HandleRaceMatchStateWaitingForPlayers()
{
	StartWaitingForPlayers();
}

void ARaceGameMode::HandleRaceMatchStateRacing()
{
	StartRacingTimer();
}

void ARaceGameMode::HandleRaceMatchStatePodium()
{
	StartPodiumTimer();
}

void ARaceGameMode::PrepareControllerForNewLap(AController* Controller)
{
	if (RaceMatchState == ERaceMatchState::RMS_Racing)
	{
		if (ARaceController* RC = Cast<ARaceController>(Controller))
		{
			RC->PrepareForNewLap(GetWorld()->GetTimeSeconds() + TimeStartDriveCountdown);
		}
	}
}

void ARaceGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();
}

void ARaceGameMode::HandleMatchHasStarted()
{
	SetRaceMatchState(ERaceMatchState::RMS_LoadingGrid); // Initialize grid before spawning players on super
	Super::HandleMatchHasStarted();
}

void ARaceGameMode::CancelLap(AController* Controller)
{
	Super::CancelLap(Controller);

	if (ARacePlayerState* PS = Controller->GetPlayerState<ARacePlayerState>())
	{
		PS->CancelLap();
	}
}

void ARaceGameMode::ControllerIsReady(AController* Controller)
{
	RestartPlayer(Controller);
}

void ARaceGameMode::SetRaceMatchState(ERaceMatchState NewState)
{
	if (RaceMatchState == NewState)
	{
		return;
	}

	RaceMatchState = NewState;
	OnRaceMatchStateSet();

	if (ARaceGameState* GS = Cast<ARaceGameState>(GetWorld()->GetGameState()))
	{
		GS->SetRaceMatchState(RaceMatchState);
	}
}
