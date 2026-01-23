// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceHUD.h"

#include "SpinningWheels/GameStates/RaceGameState.h"
#include "SpinningWheels/GameStates/TimeAttackGameState.h"
#include "UI/Slate/Overlays/Leaderboard/LeaderboardOverlay.h"

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

void ARaceHUD::BeginPlay()
{
	Super::BeginPlay();

	LeaderboardOverlay = SNew(SLeaderboardOverlay);

	if (GEngine && LeaderboardOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(LeaderboardOverlay.ToSharedRef());
	}

	if (const UWorld* World = GetWorld())
	{
		if (ATimeAttackGameState* GS = GetWorld()->GetGameState<ATimeAttackGameState>())
		{
			OnLeaderboardUpdate();
			GS->OnLeaderboardUpdate.AddDynamic(this, &ARaceHUD::OnLeaderboardUpdate);
		}
	}
}
