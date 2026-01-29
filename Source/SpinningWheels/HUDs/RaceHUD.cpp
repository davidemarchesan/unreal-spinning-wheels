// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceHUD.h"

#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/GameStates/TimeAttackGameState.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"
#include "UI/Slate/Overlays/Countdown/CountdownOverlay.h"
#include "UI/Slate/Overlays/Info/InfoOverlay.h"
#include "UI/Slate/Overlays/LapTime/LapTimeOverlay.h"
#include "UI/Slate/Overlays/Leaderboard/LeaderboardOverlay.h"
#include "UI/Slate/Overlays/ServerMessages/ServerMessagesOverlay.h"

void ARaceHUD::OnLeaderboardUpdate(const FTimeAttackLeaderboard& Leaderboard)
{
	if (LeaderboardOverlay.IsValid())
	{
		LeaderboardOverlay->OnLeaderboardUpdate(Leaderboard);
	}

	if (LapTimeOverlay.IsValid())
	{
		LapTimeOverlay->OnLeaderboardUpdate(Leaderboard);
		
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

void ARaceHUD::OnUpdateLapCountdown(int32 Seconds)
{
	if (CountdownOverlay.IsValid())
	{
		CountdownOverlay->UpdateCountdown(Seconds);
	}
}

void ARaceHUD::OnPlayerIdSet(int32 InPlayerId)
{
	PlayerId = InPlayerId;
	
	if (InfoOverlay.IsValid())
	{
		InfoOverlay->SetPlayerId(PlayerId);
	}

	if (LapTimeOverlay.IsValid())
	{
		LapTimeOverlay->SetPlayerId(PlayerId);
	}
}

void ARaceHUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeOverlays();
	InitializeDelegates();
}

void ARaceHUD::InitializeOverlays()
{
	if (GEngine == nullptr)
	{
		return;
	}
	LeaderboardOverlay = SNew(SLeaderboardOverlay);
	if (LeaderboardOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(LeaderboardOverlay.ToSharedRef());
	}

	ServerMessagesOverlay = SNew(SServerMessagesOverlay);
	if (ServerMessagesOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(ServerMessagesOverlay.ToSharedRef());
	}

	CountdownOverlay = SNew(SCountdownOverlay);
	if (CountdownOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(CountdownOverlay.ToSharedRef());
	}

	InfoOverlay = SNew(SInfoOverlay);
	if (InfoOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(InfoOverlay.ToSharedRef());
	}

	LapTimeOverlay = SNew(SLapTimeOverlay);
	if (LapTimeOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(LapTimeOverlay.ToSharedRef());
	}
}

void ARaceHUD::InitializeDelegates()
{
	if (const UWorld* World = GetWorld())
	{
		if (ATimeAttackGameState* TAGS = World->GetGameState<ATimeAttackGameState>())
		{
			OnLeaderboardUpdate(TAGS->GetLeaderboard());
			TAGS->OnLeaderboardUpdate.AddDynamic(this, &ARaceHUD::OnLeaderboardUpdate);
		}

		if (ARaceGameState* RGS = World->GetGameState<ARaceGameState>())
		{
			OnRaceMatchStateUpdate(RGS->GetRaceMatchState());
			RGS->OnRaceMatchStateUpdate.AddDynamic(this, &ARaceHUD::OnRaceMatchStateUpdate);
		}

		if (ARaceController* RC = Cast<ARaceController>(GetOwningPlayerController()))
		{
			RC->OnUpdateLapCountdown.AddDynamic(this, &ARaceHUD::OnUpdateLapCountdown);

			if (ARacePlayerState* RPS = RC->GetPlayerState<ARacePlayerState>())
			{
				OnPlayerIdSet(RPS->GetPlayerId());
				RPS->OnPlayerIdSet.AddDynamic(this, &ARaceHUD::OnPlayerIdSet);
			}
		}

	}
}
