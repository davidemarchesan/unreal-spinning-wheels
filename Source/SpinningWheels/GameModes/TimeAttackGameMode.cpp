// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeAttackGameMode.h"

#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"

void ATimeAttackGameMode::PrepareControllerForNewLap(AController* Controller)
{
	if (RaceMatchState == ERaceMatchState::RMS_Racing)
	{
		if (ARaceController* RC = Cast<ARaceController>(Controller))
		{
			RC->PrepareForNewLap(GetWorld()->GetTimeSeconds() + TimeStartDriveCountdown);
		}
	}
}

void ATimeAttackGameMode::CancelLap(AController* Controller)
{
	Super::CancelLap(Controller);

	if (ARacePlayerState* PS = Controller->GetPlayerState<ARacePlayerState>())
	{
		PS->OnCancelLap();
	}
}
