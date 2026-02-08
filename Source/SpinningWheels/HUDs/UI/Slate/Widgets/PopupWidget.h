#pragma once

class SPopupBase : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPopupBase)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

protected:
	virtual FReply ExecuteOnFocusReceive();

	virtual FReply ExecuteOnBack();

public:
	//~ SWidget overrides
	virtual bool SupportsKeyboardFocus() const override;
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	// virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	// virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	// virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	// virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	// virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	// virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
};
