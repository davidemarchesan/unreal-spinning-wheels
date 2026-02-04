#pragma once

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnMenuSlotClicked, const int8 Slot);

class SEditorBuildMenuItem : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SEditorBuildMenuItem) {}

		SLATE_ARGUMENT(int8, Slot)
		SLATE_ARGUMENT(FText, Name);

		SLATE_EVENT(FOnMenuSlotClicked, OnMenuSlotClicked)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SButton> Button;

	int8 Slot = 0;
	FOnMenuSlotClicked OnMenuSlotClicked;

public:

	FReply OnClicked();

	void SetSelected();
	void SetUnselected();
	
};