#pragma once

class STrackItem : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STrackItem)
		: _IsSelectable(false)
		{
		}

		SLATE_ARGUMENT(FText, Text)

		SLATE_ARGUMENT(bool, IsSelectable)

		SLATE_EVENT(FSimpleDelegate, OnSelected)
		SLATE_EVENT(FSimpleDelegate, OnUnselected)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	bool bIsSelectable = false;

	TSharedPtr<SBorder> Border;

	bool bSelected = false;
	bool bHovered = false;
	bool bFocused = false;

	FSimpleDelegate OnSelected;
	FSimpleDelegate OnUnselected;

	void ExecuteOnSelected();
	void ExecuteOnUnselected();

	void UpdateStyle();

public:

	void SetSelected(bool bInSelected);
	void SetHovered(bool bInHovered);
	void SetFocused(bool bInFocused);

	//~ SWidget overrides
	virtual bool SupportsKeyboardFocus() const override;
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	
	
};
