#pragma once

class SLapTimeOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLapTimeOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SOverlay> MainOverlay;

public:
	
};
