#include "MenuItemWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SMenuItem::Construct(const FArguments& InArgs)
{

	OnClicked = InArgs._OnClicked;
	
	ChildSlot[
		SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(FMargin(10.f, 5.f))
		[
			SAssignNew(TextBlock, STextBlock)
			.Text(InArgs._Text)
			.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.SemiBold.Italic.h3"))
			.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Secondary"))
			.TransformPolicy(ETextTransformPolicy::ToUpper)

		]
	];
}

FReply SMenuItem::ExecuteOnClick()
{
	if (OnClicked.IsBound())
	{
		return OnClicked.Execute();
	}

	return FReply::Unhandled();
}

void SMenuItem::UpdateStyle()
{
	if (TextBlock.IsValid() == false)
	{
		return;
	}

	if (bHovered == true || bFocused == true)
	{
		TextBlock->SetColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"));
	}
	else
	{
		TextBlock->SetColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Secondary"));
	}
}
