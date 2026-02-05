#pragma once
#include "SpinningWheels/HUDs/UI/Slate/Pages/Pages.h"

class STracksPage : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(STracksPage) {}

		SLATE_EVENT(FOnPageBack, OnPageBack)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	FOnPageBack OnPageBack;

public:

	virtual bool SupportsKeyboardFocus() const override;
	
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
};
