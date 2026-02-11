#pragma once
#include "SpinningWheels/Core/Slate.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/PopupWidget.h"

class SRaceMenuPopup : public SPopupBase
{
public:
	SLATE_BEGIN_ARGS(SRaceMenuPopup)
	{
	}

	SLATE_EVENT(FOnReply, OnBack)
	SLATE_EVENT(FOnReply, OnGoToMainMenu)

SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:

	FOnReply OnBack;

protected:

	virtual FReply ExecuteOnBack() override;

public:
};
