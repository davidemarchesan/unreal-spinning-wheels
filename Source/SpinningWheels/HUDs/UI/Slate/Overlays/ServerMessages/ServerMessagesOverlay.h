#pragma once

class SServerMessagesOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SServerMessagesOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:

	TSharedPtr<SOverlay> MainOverlay;
	TSharedPtr<STextBlock> TextBlock;

	bool bTemporary = false;
	float HideTime = 0.f;

public:

	void Show(const FText& NewText, float Seconds = 0.f);
	void Hide();
	
};