#include "LapTimeRow.h"

#include "TimeNameSlot.h"
#include "TimeSlot.h"

void SLapTimeRow::Construct(const FArguments& InArgs)
{
	const FSlateLapTimeRow LapTimeRow = InArgs._LapTimeRow;

	HorizontalBox = SNew(SHorizontalBox);

	SetLapTimeRow(LapTimeRow);

	ChildSlot[HorizontalBox.ToSharedRef()];
}

void SLapTimeRow::SetLapTimeRow(FSlateLapTimeRow NewLapTimeRow)
{

	if (HorizontalBox.IsValid() == false)
	{
		return;
	}

	HorizontalBox->ClearChildren();
	
	if (NewLapTimeRow.Name.IsEmpty() == false)
	{
		HorizontalBox->AddSlot()
					 .AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(ColumnWidth)
			[
				SNew(STimeNameSlot)
				.Title(NewLapTimeRow.Name)
			]
		];
	}

	if (NewLapTimeRow.Sectors.Num() > 0)
	{
		for (int32 i = 0; i < NewLapTimeRow.Sectors.Num(); i++)
		{
			HorizontalBox->AddSlot()
						 .AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(ColumnWidth)
				[
					SNew(STimeSlot)
					.Time(NewLapTimeRow.Sectors[i].Time)
					.TimeColor(NewLapTimeRow.Sectors[i].TimeColor)
					.Diff(NewLapTimeRow.Sectors[i].Diff)
					.DiffColor(NewLapTimeRow.Sectors[i].DiffColor)
				]
			];
		}
	}

	HorizontalBox->AddSlot()
				 .AutoWidth()
	[
		SNew(SBox)
		.WidthOverride(ColumnWidth)
		[
			SNew(STimeSlot)
			.Time(NewLapTimeRow.LapTime.Time)
			.TimeColor(NewLapTimeRow.LapTime.TimeColor)
			.Diff(NewLapTimeRow.LapTime.Diff)
			.DiffColor(NewLapTimeRow.LapTime.DiffColor)
		]
	];
}
