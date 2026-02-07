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
		.HeightOverride(300.f)
		[
			SAssignNew(Border, SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.TrackCard.Normal"))
			.Padding(FMargin(20.f, 14.f))
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(InArgs._Text)
				.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.SemiBold.Italic.h3"))
				.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
				.Justification(ETextJustify::Center)
			]
		]

	];
}

FReply STrackItem::ExecuteOnSelect()
{
	if (OnSelected.IsBound())
	{
		return OnSelected.Execute();
	}

	return FReply::Unhandled();
}

FReply STrackItem::ExecuteOnUnselect()
{
	if (OnUnselected.IsBound())
	{
		return OnUnselected.Execute();
	}

	return FReply::Unhandled();
}

void STrackItem::UpdateStyle()
{
	if (bSelected == true)
	{
		Border->SetBorderImage(FMainStyle::Get().GetBrush("Brush.TrackCard.Selected"));
	}
	else if (bHovered == true)
	{
		Border->SetBorderImage(FMainStyle::Get().GetBrush("Brush.TrackCard.Hovered"));
	}
	else
	{
		Border->SetBorderImage(FMainStyle::Get().GetBrush("Brush.TrackCard.Normal"));
	}
}

void STrackItem::SetSelected(const bool bInSelected)
{
	bSelected = bInSelected;
	UpdateStyle();
}
