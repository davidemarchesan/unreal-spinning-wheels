#pragma once
#include "FocusableWidget.h"
#include "SpinningWheels/Core/Slate.h"

class SMenuItem : public SFocusableBase
{
public:
	SLATE_BEGIN_ARGS(SMenuItem)
		{
		}

		SLATE_ARGUMENT(FText, Text)

		SLATE_EVENT(FOnReply, OnClicked)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<STextBlock> TextBlock;

	FOnReply OnClicked;

protected:

	virtual FReply ExecuteOnClick() override;

	virtual void UpdateStyle() override;

public:
	
};
