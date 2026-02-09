// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceHUD.h"

#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"
#include "UI/Slate/Overlays/Countdown/CountdownOverlay.h"
#include "UI/Slate/Overlays/Info/InfoOverlay.h"
#include "UI/Slate/Overlays/LapTime/LapTimeOverlay.h"
#include "UI/Slate/Overlays/Leaderboard/LeaderboardOverlay.h"
#include "UI/Slate/Overlays/MatchTime/MatchTimeOverlay.h"
#include "UI/Slate/Overlays/ServerMessages/ServerMessagesOverlay.h"
#include "UI/Slate/Styles/MainStyle.h"

void ARaceHUD::BeginPlay()
{
	Super::BeginPlay();

	RaceController = Cast<ARaceController>(GetOwningPlayerController());

	InitializeRootOverlay();
	InitializeDelegates();
}

void ARaceHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeDelegates();
	Super::EndPlay(EndPlayReason);
}

void ARaceHUD::ClearViewport()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
	}
}

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

	default:
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

void ARaceHUD::OnUpdateRacePlayerState(ARacePlayerState* InRacePlayerState)
{
	if (InRacePlayerState)
	{
		RacePlayerState = InRacePlayerState;

		// todo: this code repeats on initialize delegates
		OnPlayerIdSet(RacePlayerState->GetPlayerId());
		RacePlayerState->OnPlayerIdSet.AddDynamic(this, &ARaceHUD::OnPlayerIdSet);
		RacePlayerState->OnCurrentLapUpdate.AddDynamic(this, &ARaceHUD::OnCurrentLapUpdate);
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

	if (LeaderboardOverlay.IsValid())
	{
		LeaderboardOverlay->SetPlayerId(PlayerId);
	}
}

void ARaceHUD::OnCurrentLapUpdate(const FRaceLap& CurrentLap)
{
	if (LapTimeOverlay.IsValid())
	{
		LapTimeOverlay->OnCurrentLapUpdate(CurrentLap);
	}
}

void ARaceHUD::InitializeRootOverlay()
{
	if (GEngine == nullptr)
	{
		return;
	}

	RootOverlay = SNew(SOverlay)
		.Visibility(EVisibility::SelfHitTestInvisible);
	if (RootOverlay.IsValid() == false)
	{
		return;
	}

	RootCanvas = SNew(SConstraintCanvas);
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootOverlay->AddSlot()
	           .Padding(FMainStyle::Get().GetMargin("Padding.SafeArea"))
	[
		RootCanvas.ToSharedRef()
	];

	GEngine->GameViewport->AddViewportWidgetContent(RootOverlay.ToSharedRef());

	ModalOverlay = SNew(SOverlay)
		.Visibility(EVisibility::Collapsed);

	GEngine->GameViewport->AddViewportWidgetContent(ModalOverlay.ToSharedRef(), 5);

	InitializeOverlayLeaderboard();
	InitializeOverlayServerMessages();
	InitializeOverlayCountdown();
	InitializeOverlayInfo();
	InitializeOverlayLapTime();
	InitializeOverlayMatchTime();
}

void ARaceHUD::InitializeOverlayLeaderboard()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
	          .Anchors(FAnchors(0.5f, 0.5f))
	          .Alignment(FVector2D(0.5f, 0.5f))
	          .AutoSize(true)
	[
		SAssignNew(LeaderboardOverlay, SLeaderboardOverlay)
	];
}

void ARaceHUD::InitializeOverlayServerMessages()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
	          .Anchors(FAnchors(0.5f, 0.2f))
	          .Alignment(FVector2D(0.5f, 0.5f))
	          .AutoSize(true)
	[
		SAssignNew(ServerMessagesOverlay, SServerMessagesOverlay)
	];
}

void ARaceHUD::InitializeOverlayCountdown()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
	          .Anchors(FAnchors(0.5f, 0.5f))
	          .Alignment(FVector2D(0.5f, 0.5f))
	          .AutoSize(true)
	[
		SAssignNew(CountdownOverlay, SCountdownOverlay)
	];
}

void ARaceHUD::InitializeOverlayInfo()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
	          .Anchors(FAnchors(0.f, 1.f))
	          .Alignment(FVector2D(0.f, 1.f))
	          .AutoSize(true)
	[
		SAssignNew(InfoOverlay, SInfoOverlay)
	];
}

void ARaceHUD::InitializeOverlayLapTime()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
	          .Anchors(FAnchors(0.5f, 0.f))
	          .Alignment(FVector2D(0.5f, 0.f))
	          .AutoSize(true)
	[
		SAssignNew(LapTimeOverlay, SLapTimeOverlay)
	];
}

void ARaceHUD::InitializeOverlayMatchTime()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
			  .Anchors(FAnchors(1.f, 0.5f))
			  .Alignment(FVector2D(1.f, 0.5f))
			  .AutoSize(true)
	[
		SAssignNew(MatchTimeOverlay, SMatchTimeOverlay)
	];
}

void ARaceHUD::ShowModalOverlay(const TSharedPtr<SWidget>& Widget, const bool bFocus)
{
	if (ModalOverlay.IsValid() && Widget.IsValid())
	{
		ModalOverlay->ClearChildren();
		ModalOverlay->AddSlot()
		            .VAlign(VAlign_Fill)
		            .HAlign(HAlign_Fill)
		[
			Widget.ToSharedRef()
		];

		if (RaceController.IsValid())
		{
			// RaceController->BlockCursor();
		}

		ModalOverlay->SetVisibility(EVisibility::Visible);

		if (bFocus == true)
		{
			FSlateApplication::Get().SetKeyboardFocus(Widget.ToSharedRef());
		}
	}
}

void ARaceHUD::HideModalOverlay()
{
	if (ModalOverlay.IsValid())
	{
		ModalOverlay->SetVisibility(EVisibility::Collapsed);
		ModalOverlay->ClearChildren();

		if (RaceController.IsValid())
		{
			// RaceController->UnlockCursor();
		}
	}
}

void ARaceHUD::InitializeDelegates()
{
	if (const UWorld* World = GetWorld())
	{
		if (ARaceGameState* RGS = World->GetGameState<ARaceGameState>())
		{
			OnLeaderboardUpdate(RGS->GetLeaderboard());
			RGS->OnLeaderboardUpdate.AddDynamic(this, &ARaceHUD::OnLeaderboardUpdate);

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
				RPS->OnCurrentLapUpdate.AddDynamic(this, &ARaceHUD::OnCurrentLapUpdate);
			}
			else
			{
				RC->OnUpdateRacePlayerState.AddDynamic(this, &ARaceHUD::OnUpdateRacePlayerState);
			}
		}
	}
}

void ARaceHUD::DeinitializeDelegates()
{
}

void ARaceHUD::ShowLeaderboard()
{
	if (LeaderboardOverlay.IsValid())
	{
		LeaderboardOverlay->Show();
	}
}

void ARaceHUD::HideLeaderboard()
{
	if (LeaderboardOverlay.IsValid())
	{
		LeaderboardOverlay->Hide();
	}
}

void ARaceHUD::SetMatchRemainingTime(const float Seconds)
{
	if (MatchTimeOverlay.IsValid())
	{
		MatchTimeOverlay->SetMatchRemainingTime(Seconds);
	}
}
