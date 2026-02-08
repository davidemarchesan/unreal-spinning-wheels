#pragma once
#include "SpinningWheels/Core/Slate.h"

class SPlayPage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayPage)
		{
		}

		SLATE_ARGUMENT(TArray<FTrackSaveData>, Tracks)

		SLATE_EVENT(FOnReply, OnHost)
		SLATE_EVENT(FOnReply, OnJoin)

		SLATE_EVENT(FOnReply, OnPageBack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// TSharedPtr<SWidgetSwitcher> MainSwitcher;
	// TSharedPtr<SBox> ContextActionsBox;


	FOnReply OnPageBack;

public:
	virtual bool SupportsKeyboardFocus() const override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	// virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;

	// TSharedPtr<SWidget> GetFocusWidget() { return DefaultTrackItem; }
};
