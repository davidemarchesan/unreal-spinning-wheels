#include "LeaderboardOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SLeaderboardOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SAssignNew(MainOverlay, SOverlay)
		// .Visibility(EVisibility::Visible)
		.Visibility(EVisibility::Collapsed)

		+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[

			SNew(SBox)
			.WidthOverride(600.f)
			.HeightOverride(400.f)
			[
				SNew(SBorder)
				.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
				[
					SNew(SVerticalBox)
					
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString("Leaderboard"))
						.Font(FMainStyle::Get().GetFontStyle("Font.Roboto.h1"))
						.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Light"))
					]

					+ SVerticalBox::Slot()
					[
						SAssignNew(LeaderboardVerticalBox, SVerticalBox)

						+ SVerticalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString("P1 | 6.123"))
							.Font(FMainStyle::Get().GetFontStyle("Roboto.Regular.h1"))
						]

						+ SVerticalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString("P1 | 6.123"))
							.Font(FMainStyle::Get().GetFontStyle("Lato.Bold.h1"))
						]

						+ SVerticalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString("P1 | 6.123"))
						]
					]

				]
			]

		]
	];
}

void SLeaderboardOverlay::UpdateLeaderboard(TArray<FTimeAttackLeaderboardRow> NewLeaderboard)
{
	if (LeaderboardVerticalBox.IsValid())
	{
		// LeaderboardVerticalBox->ClearChildren();

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
