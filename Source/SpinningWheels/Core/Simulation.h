#pragma once

#include "Simulation.generated.h"

namespace SimulationConstants
{
	constexpr float TickFrequency = 1.f / 120.f;
	constexpr float NetworkUpdateFrequency = 1.f / 15.f;
}

USTRUCT()
struct FSimulationFrame
{
	GENERATED_BODY()
	
	UPROPERTY() int8 DriveInputValue;
	UPROPERTY() int8 BrakeInputValue;
	UPROPERTY() int8 TurnInputValue;
	UPROPERTY() int8 TurboInputValue;
};
