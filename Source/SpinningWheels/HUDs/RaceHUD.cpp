// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceHUD.h"

#include "SpinningWheels/GameStates/RaceGameState.h"
#include "UI/Slate/Overlays/Leaderboard/LeaderboardOverlay.h"

void ARaceHUD::OnLeaderboardUpdate()
{
	if (const UWorld* World = GetWorld())
	{
		if (ARaceGameState* GS = Cast<ARaceGameState>(World->GetGameState()))
		{
			TArray<float> Leaderboard = GS->GetLeaderboard();
			if (LeaderboardOverlay.IsValid())
			{
				LeaderboardOverlay->UpdateLeaderboard(Leaderboard);
			}
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
		if (ARaceGameState* GS = Cast<ARaceGameState>(World->GetGameState()))
		{
			OnLeaderboardUpdate();
			GS->OnLeaderboardUpdate.AddUObject(this, &ARaceHUD::OnLeaderboardUpdate);
		}
	}
}
