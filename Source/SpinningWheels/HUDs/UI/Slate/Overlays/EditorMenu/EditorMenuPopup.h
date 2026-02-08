#pragma once
#include "SpinningWheels/Core/Slate.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/PopupWidget.h"

class SEditorMenuPopup : public SPopupBase
{
public:
	SLATE_BEGIN_ARGS(SEditorMenuPopup)
		{
		}

		SLATE_EVENT(FOnReply, OnBack)
		SLATE_EVENT(FOnReply, OnSaveTrack)
		SLATE_EVENT(FOnReply, OnTestTrack)
		SLATE_EVENT(FOnReply, OnGoToMainMenu)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	FOnReply OnBack;

protected:

	virtual FReply ExecuteOnBack() override;

public:
};
