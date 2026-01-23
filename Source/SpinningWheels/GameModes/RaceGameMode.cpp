// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"

void ARaceGameMode::CancelLap(APlayerController* Controller)
{
	UE_LOG(LogTemp, Warning, TEXT("ARaceGameMode::CancelLap"));

	/**
	 * A player asked to cancel lap, what am i going to do?
	 * tell player state to cancel lap
	 * tell controller to spawn the car at starting point
	 */
}
