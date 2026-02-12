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
#include "UI/Slate/Overlays/RaceMenu/RaceMenuPopup.h"
#include "UI/Slate/Styles/MainStyle.h"
#include "UI/Slate/Overlays/CarStatus/CarStatusOverlay.h"

void ARaceHUD::BeginPlay()
{
	Super::BeginPlay();

	RaceController = Cast<ARaceController>(GetOwningPlayerController());

	InitializeRootOverlay();
	InitializeDelegates();

	ImReady();
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

void ARaceHUD::InitLeaderboard()
{
	if (const UWorld* World = GetWorld())
	{
		RaceGameState = World->GetGameState<ARaceGameState>();
		if (RaceGameState.IsValid())
		{
			RaceGameState->OnLeaderboardUpdate.AddUniqueDynamic(this, &ARaceHUD::OnLeaderboardUpdate);
			RaceGameState->OnRaceMatchStateUpdate.AddUniqueDynamic(this, &ARaceHUD::OnRaceMatchStateUpdate);

			OnRaceMatchStateUpdate(RaceGameState->GetRaceMatchState());
		}
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
	ShowLeaderboard();
	if (LapTimeOverlay.IsValid())
	{
		LapTimeOverlay->Hide();
	}
}

FReply ARaceHUD::OnGoToMainMenu()
{
	if (RaceController.IsValid())
	{
		RaceController->LeaveSession();
	}
	return FReply::Handled();
}

void ARaceHUD::UpdateLapCountdown(int32 Seconds)
{
	if (CountdownOverlay.IsValid())
	{
		CountdownOverlay->UpdateCountdown(Seconds);
	}
}

void ARaceHUD::SetCar(const TWeakObjectPtr<ACar> InCar)
{
	
	if (CarStatusOverlay.IsValid())
	{
		CarStatusOverlay->SetCar(InCar);
	}
}

void ARaceHUD::SetPlayerState(ARacePlayerState* InRacePlayerState)
{
	if (InRacePlayerState != nullptr)
	{
		RacePlayerState = InRacePlayerState;

		if (RacePlayerState.IsValid())
		{
			OnPlayerIdSet(RacePlayerState->GetPlayerId());
			RacePlayerState->OnPlayerIdSet.AddUniqueDynamic(this, &ARaceHUD::OnPlayerIdSet);
			RacePlayerState->OnCurrentLapUpdate.AddUniqueDynamic(this, &ARaceHUD::OnCurrentLapUpdate);
		}
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

void ARaceHUD::ImReady()
{
	bReady = true;
	if (RaceController.IsValid())
	{
		RaceController->HUDIsReady();
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
	InitializeOverlayRaceMenu();
	InitializeOverlayCarStatus();
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
	          .ZOrder(10.f)
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

void ARaceHUD::InitializeOverlayRaceMenu()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RaceMenuPopup = SNew(SRaceMenuPopup)
		.OnBack_Lambda([this]()
		{
			if (this)
			{
				HideModalOverlay();
			}
			return FReply::Handled();
		})
		.OnGoToMainMenu_Lambda([this]()
		{
			if (this)
			{
				return OnGoToMainMenu();
			}
			return FReply::Unhandled();
		});
}

void ARaceHUD::InitializeOverlayCarStatus()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
			  .Anchors(FAnchors(0.5f, 0.95f))
			  .Alignment(FVector2D(0.5f, 0.5f))
			  .AutoSize(true)
	[
		SAssignNew(CarStatusOverlay, SCarStatusOverlay)
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
			RaceController->BlockCursor();
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
			RaceController->UnlockCursor();
		}
	}
}

void ARaceHUD::InitializeDelegates()
{
	if (const UWorld* World = GetWorld())
	{
		if (RaceController.IsValid())
		{
			if (ARacePlayerState* RPS = RaceController->GetPlayerState<ARacePlayerState>())
			{
				SetPlayerState(RPS);
			}
		}
	}
}

void ARaceHUD::DeinitializeDelegates()
{
	if (RacePlayerState.IsValid())
	{
		RacePlayerState->OnPlayerIdSet.RemoveDynamic(this, &ARaceHUD::OnPlayerIdSet);
		RacePlayerState->OnCurrentLapUpdate.RemoveDynamic(this, &ARaceHUD::OnCurrentLapUpdate);
	}

	if (RaceGameState.IsValid())
	{
		RaceGameState->OnLeaderboardUpdate.RemoveDynamic(this, &ARaceHUD::OnLeaderboardUpdate);
		RaceGameState->OnRaceMatchStateUpdate.RemoveDynamic(this, &ARaceHUD::OnRaceMatchStateUpdate);
	}
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

void ARaceHUD::UpdateLeaderboard(const FTimeAttackLeaderboard& Leaderboard)
{
	OnLeaderboardUpdate(Leaderboard);
}

void ARaceHUD::SetMatchRemainingTime(const float Seconds)
{
	if (MatchTimeOverlay.IsValid())
	{
		MatchTimeOverlay->SetMatchRemainingTime(Seconds);
	}
}

void ARaceHUD::InputOpenMenu()
{
	if (RaceMenuPopup.IsValid())
	{
		ShowModalOverlay(RaceMenuPopup);
	}
}
