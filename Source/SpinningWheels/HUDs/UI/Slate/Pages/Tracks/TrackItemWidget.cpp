#include "TrackItemWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void STrackItem::Construct(const FArguments& InArgs)
{

	bIsSelectable = InArgs._IsSelectable;

	OnSelected = InArgs._OnSelected;
	OnUnselected = InArgs._OnUnselected;
	
	ChildSlot[

		SNew(SBox)
		.WidthOverride(300.f)
		[
			SAssignNew(Border, SBorder)
			.BorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 6.f, FLinearColor::Transparent, 2.f))
			.Padding(FMargin(20.f, 14.f))
			[
				SNew(STextBlock)
				.Text(InArgs._Text)
				.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.Medium.h3"))
				.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
				.Justification(ETextJustify::Center)
			]
		]

	];
}

void STrackItem::ExecuteOnSelected()
{
	if (OnSelected.IsBound())
	{
		OnSelected.Execute();
	}
}

void STrackItem::ExecuteOnUnselected()
{
	if (OnUnselected.IsBound())
	{
		OnUnselected.Execute();
	}
}

void STrackItem::UpdateStyle()
{
	if (bSelected)
	{
		Border->SetBorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Primary"), 6.f, FMainStyle::Get().GetColor("Color.Text.Light.Primary"), 2.f));
	}
	else
	{
		Border->SetBorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 6.f, FLinearColor::Transparent, 2.f));
	}
	// if (bFocused == true)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("TrackItemWidget::UpdateStyle::Focus"));
	// 	Border->SetBorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Primary"), 6.f, FMainStyle::Get().GetColor("Color.Text.Light.Primary"), 2.f));
	// }
	// else if (bHovered == true)
	// {
	// 	Border->SetBorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Primary"), 6.f, FLinearColor::Transparent, 2.f));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("TrackItemWidget::UpdateStyle::Normal"));
	// 	Border->SetBorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 6.f, FLinearColor::Transparent, 2.f));
	// }
}

void STrackItem::SetSelected(bool bInSelected)
{
	bSelected = bInSelected;
	UpdateStyle();
}

bool STrackItem::SupportsKeyboardFocus() const
{
	return true;
}

FReply STrackItem::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	bFocused = true;
	UpdateStyle();
	
	return SCompoundWidget::OnFocusReceived(MyGeometry, InFocusEvent);
}

void STrackItem::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	bFocused = false;
	UpdateStyle();
	
	SCompoundWidget::OnFocusLost(InFocusEvent);
}

FReply STrackItem::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

FReply STrackItem::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return SCompoundWidget::OnKeyUp(MyGeometry, InKeyEvent);
}

FReply STrackItem::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{

	if (IsEnabled())
	{
		if (bIsSelectable)
		{
			bSelected = !bSelected;
			UpdateStyle();

			if (bSelected)
			{
				ExecuteOnSelected();
			}
			else
			{
				ExecuteOnUnselected();
			}
		}
	}
	
	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply STrackItem::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return SCompoundWidget::OnMouseButtonUp(MyGeometry, MouseEvent);
}

void STrackItem::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bHovered = true;
	UpdateStyle();
	
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
}

void STrackItem::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bHovered = false;
	UpdateStyle();
	
	SCompoundWidget::OnMouseLeave(MouseEvent);
}
