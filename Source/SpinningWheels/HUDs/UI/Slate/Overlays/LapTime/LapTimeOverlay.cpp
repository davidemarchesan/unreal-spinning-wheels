#include "LapTimeOverlay.h"

#include "LapTimeRow.h"
#include "TimeSlot.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SLapTimeOverlay::Construct(const FArguments& InArgs)
{
	PlayerId = InArgs._PlayerId;

	const FSlateBrush* Background = FMainStyle::Get().GetBrush("Brush.Background.Black");

	ChildSlot[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		.Padding(10.f)
		[

			SNew(SBorder)
			.BorderImage(Background)
			[

				SNew(SBox)
				.Padding(10.f, 6.f)
				[

					SNew(SVerticalBox)

					// Current Match Record
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SAssignNew(RecordLapTimeRow, SLapTimeRow)
					]

					// Personal Best
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SAssignNew(PersonalBestLapTimeRow, SLapTimeRow)
					]

				]

			]

		]
	];
}

void SLapTimeOverlay::OnLeaderboardUpdate(const FTimeAttackLeaderboard& Leaderboard)
{
	const TArray<FRaceLap> Laps = Leaderboard.GetPlayersBestLap();

	if (Laps.Num() == 0)
	{
		return;
	}

	Record = Laps[0];

	// Record
	if (RecordLapTimeRow.IsValid())
	{
		FSlateLapTimeRow NewRow;
		NewRow.Name = FText::FromString("Record");
		NewRow.LapTime = FSlateTime(
			FText::FromString(Record.GetLapTimeFormat()),
			ESlateTimeColor::TC_Purple
		);

		const TArray<int32> BestSectors = Leaderboard.GetBestSectors();
		const TArray<int32> RecordSectors = Record.GetSectors();
		for (int32 i = 0; i < RecordSectors.Num(); i++)
		{
			ESlateTimeColor Color = ESlateTimeColor::TC_White;
			if (BestSectors.IsValidIndex(i) && RecordSectors[i] == BestSectors[i])
			{
				Color = ESlateTimeColor::TC_Purple;
			}
			NewRow.Sectors.Add(
				FSlateTime(
					FText::FromString(FRaceLap::FormatTime(RecordSectors[i])),
					Color
				)
			);
		}

		RecordLapTimeRow->SetLapTimeRow(NewRow);
	}

	// Personal best
	if (PersonalBestLapTimeRow.IsValid() && PlayerId != 0)
	{
		const FRaceLap* PersonalBestPtr = Laps.FindByPredicate([this](const FRaceLap& InLap)
		{
			return InLap.GetPlayerId() == PlayerId;
		});

		if (PersonalBestPtr)
		{
			PersonalBest = *PersonalBestPtr;

			FSlateLapTimeRow NewRow;
			NewRow.Name = FText::FromString("Pers. Best");
			NewRow.LapTime = FSlateTime(
				FText::FromString(PersonalBest.GetLapTimeFormat()),
				ESlateTimeColor::TC_Purple
			);

			const TArray<int32> RecordSectors = Record.GetSectors();
			const TArray<int32> PersBestSectors = PersonalBest.GetSectors();
			for (int32 i = 0; i < PersBestSectors.Num(); i++)
			{
				ESlateTimeColor Color = ESlateTimeColor::TC_White;
				if (RecordSectors.IsValidIndex(i) && PersBestSectors[i] == RecordSectors[i])
				{
					Color = ESlateTimeColor::TC_Purple;
					NewRow.Sectors.Add(
						FSlateTime(
							FText::FromString(FRaceLap::FormatTime(PersBestSectors[i])),
							Color
						)
					);
				}
				else
				{
					const int32 Diff = PersonalBest.GetSectorDiff(i, RecordSectors[i]);
					const FString DiffStr = FRaceLap::FormatDiff(Diff);
					NewRow.Sectors.Add(
					FSlateTime(
						FText::FromString(FRaceLap::FormatTime(PersBestSectors[i])),
						Color,
						FText::FromString(DiffStr),
						Diff < 0 ? ESlateTimeColor::TC_Green : ESlateTimeColor::TC_Red
					)
				);
				}
			}

			PersonalBestLapTimeRow->SetLapTimeRow(NewRow);
		}
	}
}
