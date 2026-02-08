#include "PopupWidget.h"

void SPopupBase::Construct(const FArguments& InArgs)
{
	// Child class should have custom construct
}

FReply SPopupBase::ExecuteOnFocusReceive()
{
	// Child class should implement this and decide what to do
	return FReply::Unhandled();
}

FReply SPopupBase::ExecuteOnBack()
{
	// Child class should implement this and decide what to do
	return FReply::Unhandled();
}

bool SPopupBase::SupportsKeyboardFocus() const
{
	return true;
}

FReply SPopupBase::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return ExecuteOnFocusReceive();
}

FReply SPopupBase::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Back)
	{
		return ExecuteOnBack();
	}

	return FReply::Unhandled();
}
