#include "LeaderboardOverlay.h"

void SLeaderboardOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SAssignNew(MainOverlay, SOverlay)
		.Visibility(EVisibility::Visible)
		// .Visibility(EVisibility::Collapsed)

		+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SAssignNew(LeaderboardVerticalBox, SVerticalBox)

			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("P1 | 6.123"))
			]

			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("P2 | 7.652"))
			]
		]
	];
}

void SLeaderboardOverlay::UpdateLeaderboard(TArray<FTimeAttackLeaderboardRow> NewLeaderboard)
{
	if (LeaderboardVerticalBox.IsValid())
	{
		LeaderboardVerticalBox->ClearChildren();

		for (FTimeAttackLeaderboardRow& Row : NewLeaderboard)
		{
			LeaderboardVerticalBox->AddSlot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::Printf(
					TEXT("%d | %d | %s | %f"),
					Row.Position,
					Row.PlayerId,
					*Row.PlayerName,
					Row.BestLap.GetLapTime()
				)))
			];
		}
	}
}

void SLeaderboardOverlay::Show()
{
}

void SLeaderboardOverlay::Hide()
{
}
