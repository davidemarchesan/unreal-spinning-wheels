#pragma once
#include "SpinningWheels/Core/Slate.h"

class SPlayPage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayPage)
		{
		}

		SLATE_ARGUMENT(TArray<FTrackSaveData>, Tracks)

		SLATE_EVENT(FOnReply, OnStartLANSession)
		SLATE_EVENT(FOnReply, OnJoinLANSession)
		SLATE_EVENT(FOnReply, OnStartSteamSession)

		SLATE_EVENT(FOnReply, OnPageBack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	
	FOnReply OnPageBack;

public:
	virtual bool SupportsKeyboardFocus() const override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
};
