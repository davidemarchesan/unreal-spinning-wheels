#include "CardWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SCard::Construct(const FArguments& InArgs)
{

	OnClicked = InArgs._OnClicked;
	
	ChildSlot[
		SNew(SBox)
		.WidthOverride(350.f)
		.HeightOverride(500.f)
		[
			SAssignNew(Border, SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Card.Normal"))
			.Padding(FMargin(20.f, 14.f))
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				
				SNew(STextBlock)
				.Text(InArgs._Text)
				.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.SemiBold.Italic.h2"))
				.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
				.TransformPolicy(ETextTransformPolicy::ToUpper)
				.Justification(ETextJustify::Center)
			]
		]
	];
}

FReply SCard::ExecuteOnClick()
{
	if (OnClicked.IsBound())
	{
		return OnClicked.Execute();
	}
	return FReply::Unhandled();
}

void SCard::UpdateStyle()
{
	if (bHovered)
	{
		// todo: set to mainstyle
		Border->SetBorderImage(FMainStyle::Get().GetBrush("Brush.Card.Hovered"));
	}
	else
	{
		// todo: set to mainstyle
		Border->SetBorderImage(FMainStyle::Get().GetBrush("Brush.Card.Normal"));
	}
}
