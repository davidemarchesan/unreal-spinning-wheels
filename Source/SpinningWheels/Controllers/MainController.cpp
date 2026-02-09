// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"

#include "Kismet/GameplayStatics.h"
#include "SpinningWheels/Subsystems/GameInstance/RaceServerSubsystem.h"
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

void AMainController::HostSession()
{
	// Load all tracks
	if (RaceServerSubsystem.IsValid() && TracksSubsystem.IsValid())
	{
		RaceServerSubsystem->SetTracksPlaylist(TracksSubsystem->GetTracks());

		GEngine->GameViewport->RemoveAllViewportWidgets();
		UGameplayStatics::OpenLevel(GetWorld(), "L_Race?listen");
	}
}

void AMainController::JoinSession()
{
	// 192.168.0.127
	ClientTravel("192.168.0.127", TRAVEL_Absolute);
}
