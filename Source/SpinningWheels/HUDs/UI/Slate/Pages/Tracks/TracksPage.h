#pragma once
#include "TrackItemWidget.h"
#include "SpinningWheels/HUDs/UI/Slate/Pages/Pages.h"
#include "Widgets/Layout/SWrapBox.h"

class STracksPage : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(STracksPage) {}

		SLATE_EVENT(FOnPageBack, OnPageBack)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SWrapBox> WrapBox;
	TSharedPtr<STrackItem> DefaultTrackItem;

	FOnPageBack OnPageBack;

public:

	virtual bool SupportsKeyboardFocus() const override;
	
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	TSharedPtr<SWidget> GetFocusWidget() { return DefaultTrackItem; }
	
};
