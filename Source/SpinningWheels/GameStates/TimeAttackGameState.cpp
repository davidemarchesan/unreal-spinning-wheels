// Fill out your copyright notice in the Description page of Project Settings.



#include "TimeAttackGameState.h"

#include "GameFramework/PlayerState.h"

void ATimeAttackGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerState->IsInactive() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATimeAttackGameState::AddPlayerState %d | %s"), PlayerState->GetPlayerId(), *PlayerState->GetPlayerName());
	}
}
