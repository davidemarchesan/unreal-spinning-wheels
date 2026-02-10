#include "LeaderboardOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Overlays/LapTime/LapTimeRow.h"
#include "SpinningWheels/HUDs/UI/Slate/Overlays/LapTime/TimeSlot.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SLeaderboardOverlay::Construct(const FArguments& InArgs)
{
	static FTableViewStyle ListViewStyle = FTableViewStyle().SetBackgroundBrush(
		FSlateColorBrush(FLinearColor::Transparent));

	ChildSlot[
		SAssignNew(MainBox, SBox)
		.Visibility(EVisibility::Collapsed)
		.MinDesiredWidth(900.f)
		.MinDesiredHeight(700.f)
		[
			SNew(SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
			.Padding(FMainStyle::Get().GetMargin("Padding.Box"))
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Leaderboard"))
					.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Box.Title"))
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SSpacer)
					.Size(FVector2D(1.f, 20.f))
				]

				+ SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SAssignNew(LeaderboardListView, SListView<TSharedPtr<FRaceLap>>)
					.ListItemsSource(&PlayersBestLap)
					.OnGenerateRow(this, &SLeaderboardOverlay::GenerateRow)
					.ListViewStyle(&ListViewStyle)

				]

			]
		]
	];
}

TSharedRef<ITableRow> SLeaderboardOverlay::GenerateRow(TSharedPtr<FRaceLap> Lap,
                                                       const TSharedRef<class STableViewBase>& OwningWidget)
{
	const int32 Index = PlayersBestLap.IndexOfByKey(Lap);

	FSlateLapTimeRow NewRow;
	NewRow.Name = FText::FromString(
		FString::Printf(TEXT("#%d - %d - %s"), Index + 1, Lap->GetPlayerId(), *Lap->GetPlayerName()));
	NewRow.LapTime = FSlateTime(FText::FromString(Lap->GetLapTimeFormat()));

	const TArray<int32> Sectors = Lap->GetSectors();
	for (int32 i = 0; i < Sectors.Num(); i++)
	{
		ESlateTimeColor Color = ESlateTimeColor::TC_White;
		if (Leaderboard.BestSectors.IsValidIndex(i) && Sectors[i] == Leaderboard.BestSectors[i])
		{
			Color = ESlateTimeColor::TC_Purple;
		}
		NewRow.Sectors.Add(FSlateTime(
			FText::FromString(FRaceLap::FormatTime(Sectors[i])),
			Color));
	}

	const TSharedPtr<SLapTimeRow> LapTimeRow = SNew(SLapTimeRow)
		.LapTimeRow(NewRow);

	return SNew(STableRow<TSharedPtr<FRaceLap>>, OwningWidget)
		[
			LapTimeRow.ToSharedRef()
		];
}

void SLeaderboardOverlay::OnLeaderboardUpdate(FTimeAttackLeaderboard InLeaderboard)
{
	if (LeaderboardListView.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SLeaderboardOverlay::OnLeaderboardUpdate LeaderboardListView not valid"));
		return;
	}

	PlayersBestLap.Empty();
	const TArray<FRaceLap> Laps = InLeaderboard.GetPlayersBestLap();

	UE_LOG(LogTemp, Warning, TEXT("SLeaderboardOverlay::OnLeaderboardUpdate laps %d and best lap time %d"), Laps.Num(), InLeaderboard.BestLapTime);

	if (Laps.Num() == 0)
	{
		return;
	}

	Leaderboard = InLeaderboard;

	Record = Laps[0];

	// Creating shared ptrs for list view
	for (int i = 0; i < Laps.Num(); i++)
	{
		PlayersBestLap.Add(MakeShared<FRaceLap>(Laps[i]));
	}

	LeaderboardListView->RequestListRefresh();
}

void SLeaderboardOverlay::Show()
{
	if (MainBox.IsValid())
	{
		MainBox->SetVisibility(EVisibility::Visible);
	}
}

void SLeaderboardOverlay::Hide()
{
	if (MainBox.IsValid())
	{
		MainBox->SetVisibility(EVisibility::Collapsed);
	}
}
