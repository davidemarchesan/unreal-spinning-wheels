#pragma once
#include "SpinningWheels/Core/Slate.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/FocusableWidget.h"

class STrackItem : public SFocusableBase
{
public:
	SLATE_BEGIN_ARGS(STrackItem)
		: _IsSelectable(false)
		{
		}

		SLATE_ARGUMENT(FText, Text)

		SLATE_ARGUMENT(bool, IsSelectable)

		SLATE_EVENT(FOnReply, OnSelected)
		SLATE_EVENT(FOnReply, OnUnselected)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SBorder> Border;
	
	FOnReply OnSelected;
	FOnReply OnUnselected;

protected:

	virtual FReply ExecuteOnSelect() override;
	virtual FReply ExecuteOnUnselect() override;

	virtual void UpdateStyle() override;

public:

	void SetSelected(const bool bInSelected);
	
};
