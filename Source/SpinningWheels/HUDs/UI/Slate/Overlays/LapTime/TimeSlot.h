#pragma once

class STimeSlot : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(STimeSlot) {}
		SLATE_ARGUMENT(FText, Time)
		SLATE_ARGUMENT(FLinearColor, TimeColor)
		SLATE_ARGUMENT(FText, Diff)
		SLATE_ARGUMENT(FLinearColor, DiffColor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SOverlay> MainOverlay;

public:
	
};