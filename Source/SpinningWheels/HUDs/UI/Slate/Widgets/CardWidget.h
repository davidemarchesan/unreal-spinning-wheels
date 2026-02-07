#pragma once
#include "FocusableWidget.h"
#include "SpinningWheels/Core/Slate.h"

class SCard : public SFocusableBase
{
public:
	SLATE_BEGIN_ARGS(SCard)
		{
		}

		SLATE_ARGUMENT(FText, Text)

		SLATE_EVENT(FOnReply, OnClicked)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SBorder> Border;

	FOnReply OnClicked;

protected:

	virtual FReply ExecuteOnClick() override;

	virtual void UpdateStyle() override;

public:
	
};
