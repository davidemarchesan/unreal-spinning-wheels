#pragma once
#include "SpinningWheels/HUDs/UI/Slate/Pages/Pages.h"

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnMainMenuItemSelected, const EMenuPage);

class SMainMenuItemWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMainMenuItemWidget) {}

		SLATE_ARGUMENT(FText, Text)
		SLATE_ARGUMENT(EMenuPage, Page)

		SLATE_EVENT(FOnMainMenuItemSelected, OnMainMenuItemSelected)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	
	TSharedPtr<STextBlock> TextBlock;

	EMenuPage Page = EMenuPage::MP_Tracks;
	FOnMainMenuItemSelected OnMainMenuItemSelected;

	void OnFocus();
	void OnBlur();

public:

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
};
