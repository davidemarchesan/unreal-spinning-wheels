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

void SLeaderboardOverlay::UpdateLeaderboard(TArray<float> NewLeaderboard)
{
	if (LeaderboardVerticalBox.IsValid())
	{
		LeaderboardVerticalBox->ClearChildren();

		for (int i = 0; i < NewLeaderboard.Num(); i++)
		{
			LeaderboardVerticalBox->AddSlot()
			[
				SNew(STextBlock)
				.Text(FText::AsNumber(NewLeaderboard[i]))
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
