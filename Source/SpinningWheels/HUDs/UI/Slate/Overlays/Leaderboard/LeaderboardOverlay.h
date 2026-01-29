#pragma once

#include "SpinningWheels/Core/Leaderboard.h"
#include "Widgets/SCompoundWidget.h"

class SLeaderboardOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLeaderboardOverlay) {}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SOverlay> MainOverlay;
	// TSharedPtr<SVerticalBox> LeaderboardVerticalBox;
	TSharedPtr<SListView<TSharedPtr<FRaceLap>>> LeaderboardListView;

	TArray<TSharedPtr<FRaceLap>> PlayersBestLap;
	TSharedRef<ITableRow> GenerateRow(TSharedPtr<FRaceLap> Lap, const TSharedRef< class STableViewBase >& OwningWidget);

	FTimeAttackLeaderboard Leaderboard;

public:

	void OnLeaderboardUpdate(FTimeAttackLeaderboard InLeaderboard);
	void Show();
	void Hide();
	
};
