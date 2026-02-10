#pragma once

class SMatchTimeOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMatchTimeOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:

	TSharedPtr<STextBlock> TextBlock;

	int32 RemainingSecondsCeil = 0;
	float RemainingSeconds = 0.f;

	void UpdateVisuals();

public:

	void SetMatchRemainingTime(const float Seconds);
	
};
