#pragma once
#include "MainMenuItemWidget.h"

class SMainMenuPage : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMainMenuPage) {}

		SLATE_EVENT(FOnMainMenuItemSelected, OnMainMenuItemSelected)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

public:

	virtual bool SupportsKeyboardFocus() const override;
	
};
