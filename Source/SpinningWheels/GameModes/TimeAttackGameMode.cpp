// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGameMode.h"

#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"

void ATimeAttackGameMode::CancelLap(ARaceController* Controller)
{
	Super::CancelLap(Controller);

	if (ARacePlayerState* PS = Controller->GetRacePlayerState())
	{
		PS->OnCancelLap();
	}
}
