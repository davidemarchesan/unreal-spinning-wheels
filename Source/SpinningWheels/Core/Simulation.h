#pragma once

namespace SimulationConstants
{
	constexpr float TickFrequency = 1.f / 120.f;
}

struct FSimulationFrame
{
	int8 DriveInputValue;
	int8 BrakeInputValue;
	int8 TurnInputValue;
	int8 TurboInputValue;
};
