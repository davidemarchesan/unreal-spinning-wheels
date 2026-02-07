#pragma once
#include "TrackItemWidget.h"
#include "SpinningWheels/Core/Track.h"
#include "Widgets/Layout/SWrapBox.h"
#include "SpinningWheels/Core/Slate.h"

class STracksEditOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STracksEditOverlay)
		{
		}

		SLATE_ARGUMENT(TArray<FTrackSaveData>, Tracks)

		SLATE_EVENT(FOnTrackSelected, OnTrackSelected)
		SLATE_EVENT(FOnTrackUnselected, OnTrackUnselected)

		SLATE_EVENT(FOnEditTrack, OnEditTrack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SWrapBox> WrapBox;
	TArray<TSharedPtr<STrackItem>> TrackItems;
	TSharedPtr<STrackItem> DefaultTrackItem;

	TArray<FTrackSaveData> Tracks;

	FOnTrackSelected OnTrackSelected;
	FOnTrackUnselected OnTrackUnselected;

	void ExecuteOnTrackSelected(const int32 Index);
	void ExecuteOnTrackUnselected(const int32 Index);

public:

	void DeselectTracks(int32 Except = INDEX_NONE);
};
