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
	TArray<TSharedPtr<STrackItem>> TrackItems;
	TSharedPtr<STrackItem> DefaultTrackItem;

	TSharedPtr<SBox> ContextActionsBox;

	FOnPageBack OnPageBack;

	void OnTrackSelected(const int32 Index);
	void OnTrackUnselected(const int32 Index);

	void DeselectTracks(int32 Except = INDEX_NONE);

	void UpdateContextActions();

	void SetSelectedTrackIndex(const int32 Index);
	int32 SelectedTrackIndex = INDEX_NONE;

public:

	virtual bool SupportsKeyboardFocus() const override;
	
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	// virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	TSharedPtr<SWidget> GetFocusWidget() { return DefaultTrackItem; }
	
};
