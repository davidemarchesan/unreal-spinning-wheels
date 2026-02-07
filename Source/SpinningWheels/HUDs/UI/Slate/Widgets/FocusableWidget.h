#pragma once

class SFocusableBase : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFocusableBase)
		{
		}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

protected:

	bool bIsSelectable = false;
	
	bool bSelected = false;
	bool bHovered = false;
	bool bFocused = false;
	
	virtual FReply ExecuteOnClick();
	virtual FReply ExecuteOnSelect();
	virtual FReply ExecuteOnUnselect();
	
	virtual void UpdateStyle();

public:
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
