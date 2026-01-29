#pragma once

class STimeNameSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STimeNameSlot)
		{
		}

		SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SOverlay> MainOverlay;

public:
};
