// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackPlayerState.h"

#include "SpinningWheels/GameStates/TimeAttackGameState.h"

ATimeAttackGameState* ATimeAttackPlayerState::GetTimeAttackGameState()
{
	return GetWorld()->GetGameState<ATimeAttackGameState>();
}

void ATimeAttackPlayerState::OnNewBestLap(FRaceLap Lap)
{
	Super::OnNewBestLap(Lap);

	UE_LOG(LogTemp, Warning, TEXT("Time attack: new player best"));
	if (ATimeAttackGameState* GS = GetTimeAttackGameState())
	{
		GS->OnNewBestLap(this, Lap);
	}
}
