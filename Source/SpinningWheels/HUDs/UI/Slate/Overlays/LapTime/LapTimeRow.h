#pragma once

#include "SpinningWheels/Core/Lap.h"

class SLapTimeRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLapTimeRow)
		{
		}

		SLATE_ARGUMENT(FSlateLapTimeRow, LapTimeRow)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SHorizontalBox> HorizontalBox;

	const float ColumnWidth = 200.f;

public:

	void SetLapTimeRow(FSlateLapTimeRow NewLapTimeRow);
	
};
