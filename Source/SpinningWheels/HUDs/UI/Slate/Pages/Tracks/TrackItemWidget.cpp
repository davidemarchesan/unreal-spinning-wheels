#include "TrackItemWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void STrackItem::Construct(const FArguments& InArgs)
{
	
	ChildSlot[

		SNew(SBox)
		.WidthOverride(300.f)
		[
			SAssignNew(Border, SBorder)
			.BorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 6.f, FLinearColor::Transparent, 2.f))
			// .BorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 6.f, FMainStyle::Get().GetColor("Color.Text.Light.Primary"), 2.f))
			.Padding(FMargin(20.f, 14.f))
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Track Name")))
				.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.Medium.h3"))
				.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
				.Justification(ETextJustify::Center)
			]
		]

	];
}

void STrackItem::UpdateStyle()
{
	if (bFocused == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("TrackItemWidget::UpdateStyle::Focus"));
		Border->SetBorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 6.f, FMainStyle::Get().GetColor("Color.Text.Light.Primary"), 2.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TrackItemWidget::UpdateStyle::Normal"));
		Border->SetBorderImage(new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 6.f, FLinearColor::Transparent, 2.f));
	}
}

bool STrackItem::SupportsKeyboardFocus() const
{
	return true;
}

FReply STrackItem::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("TrackItemWidget::OnFocusReceived"));
	bFocused = true;
	UpdateStyle();
	return SCompoundWidget::OnFocusReceived(MyGeometry, InFocusEvent);
}

void STrackItem::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("TrackItemWidget::OnFocusLost"));
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
	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply STrackItem::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return SCompoundWidget::OnMouseButtonUp(MyGeometry, MouseEvent);
}

void STrackItem::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
}

void STrackItem::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseLeave(MouseEvent);
}
