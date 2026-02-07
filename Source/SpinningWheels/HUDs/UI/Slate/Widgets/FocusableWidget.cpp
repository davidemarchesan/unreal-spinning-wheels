#include "FocusableWidget.h"

void SFocusableBase::Construct(const FArguments& InArgs)
{
	// Child class should have custom construct
}

FReply SFocusableBase::ExecuteOnClick()
{
	// Child class should implement this and decide what to do
	return FReply::Unhandled();
}

FReply SFocusableBase::ExecuteOnSelect()
{
	// Child class should implement this and decide what to do
	return FReply::Unhandled();
}

FReply SFocusableBase::ExecuteOnUnselect()
{
	// Child class should implement this and decide what to do
	return FReply::Unhandled();
}

void SFocusableBase::UpdateStyle()
{
	// Child class should have custom style
}

bool SFocusableBase::SupportsKeyboardFocus() const
{
	return true;
}

FReply SFocusableBase::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return SCompoundWidget::OnFocusReceived(MyGeometry, InFocusEvent);
}

void SFocusableBase::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SCompoundWidget::OnFocusLost(InFocusEvent);
}

FReply SFocusableBase::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		return ExecuteOnClick();
	}

	return FReply::Unhandled();
}

FReply SFocusableBase::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return SCompoundWidget::OnKeyUp(MyGeometry, InKeyEvent);
}

FReply SFocusableBase::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsEnabled())
	{
		if (bIsSelectable == true)
		{
			bSelected = !bSelected;
			UpdateStyle();

			if (bSelected)
			{
				ExecuteOnSelect();
			}
			else
			{
				ExecuteOnUnselect();
			}
		}
		else
		{
			return ExecuteOnClick();
		}
	}

	return FReply::Unhandled();
}

FReply SFocusableBase::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return SCompoundWidget::OnMouseButtonUp(MyGeometry, MouseEvent);
}

void SFocusableBase::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsEnabled())
	{
		bHovered = true;
		UpdateStyle();
	}
}

void SFocusableBase::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	if (IsEnabled())
	{
		bHovered = false;
		UpdateStyle();
	}
}
