#pragma once
#include "LapTimeRow.h"
#include "SpinningWheels/Core/Leaderboard.h"

class SLapTimeOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLapTimeOverlay) {}
		SLATE_ARGUMENT(int32, PlayerId)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SBorder> MainBorder;
	TSharedPtr<SLapTimeRow> RecordLapTimeRow;
	TSharedPtr<SLapTimeRow> PersonalBestLapTimeRow;
	TSharedPtr<SLapTimeRow> CurrentLapTimeRow;

	FRaceLap Record = FRaceLap();
	FRaceLap PersonalBest = FRaceLap();
	FRaceLap LastPersonalBest = FRaceLap();
	FRaceLap Current = FRaceLap();

	FTimeAttackLeaderboard Leaderboard;
	
	int32 PlayerId = 0;

public:

	void SetPlayerId(int32 InPlayerId) { PlayerId = InPlayerId; };

	void OnLeaderboardUpdate(const FTimeAttackLeaderboard& InLeaderboard);
	void OnCurrentLapUpdate(const FRaceLap& CurrentLap);

	void Hide();
	
};
