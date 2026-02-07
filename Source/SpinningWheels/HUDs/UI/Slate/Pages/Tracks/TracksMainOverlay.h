#pragma once
#include "SpinningWheels/Core/Slate.h"

class STracksMainOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STracksMainOverlay)
	{
	}

		SLATE_EVENT(FOnReply, OnCreateTrack)
		SLATE_EVENT(FOnReply, OnGoToTracks)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:


public:
};
