#pragma once
#include "ModalBaseWidget.h"

DECLARE_DELEGATE_RetVal(FReply, FOnConfirm)

class SModalConfirm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModalConfirm)
		{
		}

		SLATE_ARGUMENT(FText, Title);
		SLATE_ARGUMENT(FText, Text);

		SLATE_EVENT(FOnConfirm, OnConfirm)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SBox> MainBox;
};
