#include "LapTimeOverlay.h"

#include "LapTimeRow.h"
#include "TimeSlot.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SLapTimeOverlay::Construct(const FArguments& InArgs)
{
	PlayerId = InArgs._PlayerId;
	
	ChildSlot[
		
		SNew(SBorder)
		.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
		.Padding(FMainStyle::Get().GetMargin("Padding.Box"))
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

				// Current
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(CurrentLapTimeRow, SLapTimeRow)
				]

			]

		]


	];
}

void SLapTimeOverlay::OnLeaderboardUpdate(const FTimeAttackLeaderboard& InLeaderboard)
{
	Leaderboard = InLeaderboard;

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
			LastPersonalBest = PersonalBest;
			PersonalBest = *PersonalBestPtr;

			// At the first pers best lap, last personal best would be zero
			if (LastPersonalBest.bClosed == false)
			{
				LastPersonalBest = PersonalBest;
			}

			FSlateLapTimeRow NewRow;
			NewRow.Name = FText::FromString("Pers. Best");

			ESlateTimeColor LapColor = ESlateTimeColor::TC_White;
			if (PersonalBest == Record)
			{
				LapColor = ESlateTimeColor::TC_Purple;
			}
			const int32 LapDiff = PersonalBest.GetLapTimeDiff(Record.GetLapTime());
			const FString LapDiffStr = FRaceLap::FormatDiff(LapDiff);
			NewRow.LapTime = FSlateTime(
				FText::FromString(PersonalBest.GetLapTimeFormat()),
				LapColor,
				FText::FromString(LapDiffStr),
				LapDiff < 0 ? ESlateTimeColor::TC_Green : ESlateTimeColor::TC_Red
			);

			const TArray<int32> BestSectors = Leaderboard.GetBestSectors();
			const TArray<int32> RecordSectors = Record.GetSectors();
			const TArray<int32> PersBestSectors = PersonalBest.GetSectors();
			for (int32 i = 0; i < PersBestSectors.Num(); i++)
			{
				ESlateTimeColor SectorColor = ESlateTimeColor::TC_White;
				if (PersBestSectors[i] == BestSectors[i])
				{
					SectorColor = ESlateTimeColor::TC_Purple;
				}

				const int32 SectorDiff = PersonalBest.GetSectorDiff(i, RecordSectors[i]);
				const FString SectorDiffStr = FRaceLap::FormatDiff(SectorDiff);
				NewRow.Sectors.Add(
					FSlateTime(
						FText::FromString(FRaceLap::FormatTime(PersBestSectors[i])),
						SectorColor,
						FText::FromString(SectorDiffStr),
						SectorDiff < 0 ? ESlateTimeColor::TC_Green : ESlateTimeColor::TC_Red
					)
				);
			}

			PersonalBestLapTimeRow->SetLapTimeRow(NewRow);
		}
	}
}

void SLapTimeOverlay::OnCurrentLapUpdate(const FRaceLap& CurrentLap)
{
	if (CurrentLapTimeRow.IsValid() == false)
	{
		return;
	}

	Current = CurrentLap;

	FSlateLapTimeRow NewRow;
	NewRow.Name = FText::FromString("Current");

	if (CurrentLap.bClosed == true)
	{
		ESlateTimeColor LapColor = ESlateTimeColor::TC_White;
		if (Current == Record)
		{
			LapColor = ESlateTimeColor::TC_Purple;
		}
		else if (Current < LastPersonalBest)
		{
			LapColor = ESlateTimeColor::TC_Green;
		}
		const int32 LapDiff = Current.GetLapTimeDiff(LastPersonalBest.GetLapTime());
		const FString LapDiffStr = FRaceLap::FormatDiff(LapDiff);
		NewRow.LapTime = FSlateTime(
			FText::FromString(CurrentLap.GetLapTimeFormat()),
			LapColor,
			FText::FromString(LapDiffStr),
			LapDiff < 0 ? ESlateTimeColor::TC_Green : ESlateTimeColor::TC_Red
		);
	}

	const TArray<int32> CurrentSectors = Current.GetSectors();
	const TArray<int32> BestSectors = Leaderboard.GetBestSectors();
	const TArray<int32> PersBestSectors = LastPersonalBest.GetSectors();
	for (int32 i = 0; i < CurrentSectors.Num(); i++)
	{
		ESlateTimeColor SectorColor = ESlateTimeColor::TC_White;
		if (BestSectors.Num() >= CurrentSectors.Num() && CurrentSectors[i] < BestSectors[i])
		{
			SectorColor = ESlateTimeColor::TC_Purple;
		}

		FSlateTime SlateTimeSlot = FSlateTime(
			FText::FromString(FRaceLap::FormatTime(CurrentSectors[i])),
			SectorColor
		);

		if (PersBestSectors.Num() >= CurrentSectors.Num())
		{
			const int32 SectorDiff = CurrentLap.GetSectorDiff(i, PersBestSectors[i]);
			const FString SectorDiffStr = FRaceLap::FormatDiff(SectorDiff);
			SlateTimeSlot.Diff = FText::FromString(SectorDiffStr);
			SlateTimeSlot.DiffColor = SectorDiff < 0 ? ESlateTimeColor::TC_Green : ESlateTimeColor::TC_Red;
		}

		NewRow.Sectors.Add(SlateTimeSlot);
	}


	CurrentLapTimeRow->SetLapTimeRow(NewRow);
}
