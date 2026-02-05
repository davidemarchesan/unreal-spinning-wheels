#include "MainMenuItemWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SMainMenuItemWidget::Construct(const FArguments& InArgs)
{

	Page = InArgs._Page;
	
	OnMainMenuItemSelected = InArgs._OnMainMenuItemSelected;
	
	ChildSlot[
		SAssignNew(TextBlock, STextBlock)
		.Text(InArgs._Text)
		.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.SemiBold.Italic.h2"))
		.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Secondary"))
		.TransformPolicy(ETextTransformPolicy::ToUpper)
	];
}

void SMainMenuItemWidget::OnFocus()
{
	if (TextBlock.IsValid())
	{
		TextBlock->SetColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"));
	}
}

void SMainMenuItemWidget::OnBlur()
{
	if (TextBlock.IsValid())
	{
		TextBlock->SetColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Secondary"));
	}
}

void SMainMenuItemWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnFocus();
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
}

void SMainMenuItemWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	OnBlur();
	SCompoundWidget::OnMouseLeave(MouseEvent);
}

FReply SMainMenuItemWidget::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	OnFocus();
	return SCompoundWidget::OnFocusReceived(MyGeometry, InFocusEvent);
}

void SMainMenuItemWidget::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	OnBlur();
	SCompoundWidget::OnFocusLost(InFocusEvent);
}

FReply SMainMenuItemWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (OnMainMenuItemSelected.IsBound())
	{
		FReply Reply = OnMainMenuItemSelected.Execute(Page);
	}

	return FReply::Unhandled();
}
