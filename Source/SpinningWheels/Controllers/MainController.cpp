// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"

#include "Kismet/GameplayStatics.h"
#include "SpinningWheels/Subsystems/GameInstance/RaceServerSubsystem.h"
#include "SpinningWheels/Subsystems/GameInstance/RaceSessionSubsystem.h"
#include "SpinningWheels/Subsystems/GameInstance/TracksSubsystem.h"

void AMainController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

	GameInstance = GetGameInstance();
	if (GameInstance.IsValid())
	{
		TracksSubsystem = GameInstance->GetSubsystem<UTracksSubsystem>();
		RaceServerSubsystem = GameInstance->GetSubsystem<URaceServerSubsystem>();
	}
}

void AMainController::StartLANSession()
{
	if (GameInstance.IsValid())
	{
		if (URaceSessionSubsystem* RaceSessionSubsystem = GameInstance->GetSubsystem<URaceSessionSubsystem>())
		{

			// Load all tracks (just for testing) (playlist in future)
			if (RaceServerSubsystem.IsValid() && TracksSubsystem.IsValid())
			{
				RaceServerSubsystem->SetTracksPlaylist(TracksSubsystem->GetTracks());
			}
			
			RaceSessionSubsystem->StartLANSession();
		}
	}
}

void AMainController::JoinLANSession()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
	}
	ClientTravel("192.168.0.127", TRAVEL_Absolute);
}

void AMainController::StartSteamSession()
{
	// Load all tracks (just for testing) (playlist in future)
	if (RaceServerSubsystem.IsValid() && TracksSubsystem.IsValid())
	{
		RaceServerSubsystem->SetTracksPlaylist(TracksSubsystem->GetTracks());
	}
	
	if (GameInstance.IsValid())
	{
		if (URaceSessionSubsystem* RaceSessionSubsystem = GameInstance->GetSubsystem<URaceSessionSubsystem>())
		{
			RaceSessionSubsystem->StartSteamSession();
		}
	}
}
