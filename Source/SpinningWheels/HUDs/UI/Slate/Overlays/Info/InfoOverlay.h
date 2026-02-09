#pragma once

class SInfoOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SInfoOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<STextBlock> PlayerIdTextBlock;

public:

	void SetPlayerId(int32 PlayerId);
	
};
