#pragma once
#include "TracksEditOverlay.h"
#include "TracksMainOverlay.h"
#include "SpinningWheels/Core/Track.h"

class STracksPage : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(STracksPage) {}

		SLATE_ARGUMENT(TArray<FTrackSaveData>, Tracks)

		SLATE_EVENT(FOnReply, OnCreateTrack)
		SLATE_EVENT(FOnEditTrack, OnEditTrack)
		
		SLATE_EVENT(FOnReply, OnPageBack)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SWidgetSwitcher> MainSwitcher;
	TSharedPtr<STracksMainOverlay> TracksMainOverlay;
	TSharedPtr<STracksEditOverlay> TracksEditOverlay;
	TSharedPtr<SBox> ContextActionsBox;

	bool bInSubPage = false;
	FReply GoToTracksSubPage();
	
	TArray<FTrackSaveData> Tracks;

	FOnReply OnPageBack;

	void UpdateContextActions();
	
	FOnEditTrack OnEditTrack;

	FReply ExecuteEditTrack();

	void OnTrackSelected(const int32 Index);
	void OnTrackUnselected(const int32 Index);

	void SetSelectedTrackIndex(const int32 Index);
	int32 SelectedTrackIndex = INDEX_NONE;

public:
	
	virtual bool SupportsKeyboardFocus() const override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	// virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	// TSharedPtr<SWidget> GetFocusWidget() { return DefaultTrackItem; }
	
};
