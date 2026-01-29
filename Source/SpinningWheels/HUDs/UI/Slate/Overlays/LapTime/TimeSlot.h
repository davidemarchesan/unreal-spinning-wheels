#pragma once
#include "SpinningWheels/Core/Lap.h"

class STimeSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STimeSlot)
		{
		}

		SLATE_ARGUMENT(FText, Time)
		SLATE_ARGUMENT(ESlateTimeColor, TimeColor)
		SLATE_ARGUMENT(FText, Diff)
		SLATE_ARGUMENT(ESlateTimeColor, DiffColor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SOverlay> MainOverlay;

	FLinearColor GetColor(ESlateTimeColor InColor);

public:
};
