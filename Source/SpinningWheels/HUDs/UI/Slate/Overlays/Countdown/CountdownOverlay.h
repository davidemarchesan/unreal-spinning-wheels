#pragma once

class SCountdownOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCountdownOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SOverlay> MainOverlay;
	TSharedPtr<STextBlock> TextBlock;


	void Hide();

public:

	void UpdateCountdown(const int Seconds);
	
};
