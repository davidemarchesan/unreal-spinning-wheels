#pragma once

#include "Widgets/SCompoundWidget.h"

class SLeaderboardOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLeaderboardOverlay) {}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SOverlay> MainOverlay;
	TSharedPtr<SVerticalBox> LeaderboardVerticalBox;

public:

	void UpdateLeaderboard(TArray<float> NewLeaderboard);
	void Show();
	void Hide();

	
};
