// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"

#include "SpinningWheels/Actors/TrackGrid.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"
#include "SpinningWheels/Subsystems/GameInstance/RaceServerSubsystem.h"
#include "SpinningWheels/Subsystems/GameInstance/TracksSubsystem.h"

void ARaceGameMode::InitializeGrid()
{

	SetRaceMatchState(ERaceMatchState::RMS_LoadingGrid);
	
	TrackGrid = GetWorld()->SpawnActor<ATrackGrid>(DefaultTrackGridClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (URaceServerSubsystem* RaceServerSubsystem = GameInstance->GetSubsystem<URaceServerSubsystem>())
		{
			// TEST ONLY
			if (const UTracksSubsystem* TracksSubsystem = GameInstance->GetSubsystem<UTracksSubsystem>())
			{
				RaceServerSubsystem->SetTracksPlaylist(TracksSubsystem->GetTracks());
			}
			// END TEST ONLY
			
			const FTrackSaveData TrackToLoadSaveData = RaceServerSubsystem->GetCurrentTrack();
			CurrentTrack = FTrack(TrackToLoadSaveData.Id, TrackToLoadSaveData.Name);

			TrackGrid->Initialize(TrackConstants::Size, TrackConstants::Size, TrackToLoadSaveData);
		}
	}

	StartWaitingForPlayers();
}

void ARaceGameMode::StartWaitingForPlayers()
{
	SetRaceMatchState(ERaceMatchState::RMS_WaitingForPlayers);
	
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
	GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimer, this, &ARaceGameMode::StopWaitingForPlayers,
	                                       TimeWaitingForPlayers, false);
	
}

void ARaceGameMode::StopWaitingForPlayers()
{
	SetRaceMatchState(ERaceMatchState::RMS_Racing);
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
}

void ARaceGameMode::OnNewRaceMatchState()
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
	}
}

void ARaceGameMode::HandleRaceMatchStateLoadingGrid()
{
}

void ARaceGameMode::HandleRaceMatchStateWaitingForPlayers()
{
}

void ARaceGameMode::HandleRaceMatchStateRacing()
{
}

void ARaceGameMode::HandleRaceMatchStatePodium()
{
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
	InitializeGrid(); // Initialize grid before spawning players on super
	
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

	if (ARaceGameState* GS = Cast<ARaceGameState>(GetWorld()->GetGameState()))
	{
		GS->SetRaceMatchState(RaceMatchState);
	}
}
