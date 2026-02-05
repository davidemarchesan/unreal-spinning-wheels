#pragma once
#include "SpinningWheels/Core/Track.h"

class SEditorTrackDataOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditorTrackDataOverlay)
		{
		}


	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<STextBlock> IdTextBlock;
	TSharedPtr<STextBlock> NameTextBlock;

public:

	void Update(const FTrack& CurrentTrack);
};
