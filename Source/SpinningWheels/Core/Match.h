#pragma once

#include "Match.generated.h"

UENUM()
enum class ERaceMatchState : uint8
{
	RMS_WaitingForPlayers,
	RMS_Racing,
	RMS_Podium,
};
