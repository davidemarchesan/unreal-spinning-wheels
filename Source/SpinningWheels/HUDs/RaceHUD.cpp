// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceHUD.h"

#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/GameStates/TimeAttackGameState.h"
#include "UI/Slate/Overlays/Leaderboard/LeaderboardOverlay.h"
#include "UI/Slate/Overlays/ServerMessages/ServerMessagesOverlay.h"

void ARaceHUD::OnLeaderboardUpdate()
{
	if (ATimeAttackGameState* GS = GetWorld()->GetGameState<ATimeAttackGameState>())
	{
		if (LeaderboardOverlay.IsValid())
		{
			LeaderboardOverlay->UpdateLeaderboard(GS->GetLeaderboard());
		}
	}
}

void ARaceHUD::OnRaceMatchStateUpdate(ERaceMatchState NewState)
{

	switch (NewState)
	{
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

void ARaceHUD::HandleRaceMatchStateWaitingForPlayers()
{
	if (ServerMessagesOverlay.IsValid())
	{
		ServerMessagesOverlay->Show(FText::FromString("Waiting for players. Match will start soon."));
	}
}

void ARaceHUD::HandleRaceMatchStateRacing()
{
	if (ServerMessagesOverlay.IsValid())
	{
		ServerMessagesOverlay->Hide();
	}
}

void ARaceHUD::HandleRaceMatchStatePodium()
{
}

void ARaceHUD::BeginPlay()
{
	Super::BeginPlay();

	LeaderboardOverlay = SNew(SLeaderboardOverlay);
	if (GEngine && LeaderboardOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(LeaderboardOverlay.ToSharedRef());
	}

	ServerMessagesOverlay = SNew(SServerMessagesOverlay);
	if (GEngine && ServerMessagesOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(ServerMessagesOverlay.ToSharedRef());
	}

	if (const UWorld* World = GetWorld())
	{
		if (ATimeAttackGameState* GS = GetWorld()->GetGameState<ATimeAttackGameState>())
		{
			OnLeaderboardUpdate();
			GS->OnLeaderboardUpdate.AddDynamic(this, &ARaceHUD::OnLeaderboardUpdate);
		}

		if (ARaceGameState* GS = GetWorld()->GetGameState<ARaceGameState>())
		{
			OnRaceMatchStateUpdate(GS->GetRaceMatchState());
			GS->OnRaceMatchStateUpdate.AddDynamic(this, &ARaceHUD::OnRaceMatchStateUpdate);
		}
	}
}
