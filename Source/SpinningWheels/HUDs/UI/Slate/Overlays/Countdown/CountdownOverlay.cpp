#include "CountdownOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SCountdownOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo Font = FMainStyle::Get().GetFontStyle("Font.JetBrains.Bold.h1");
	Font.Size = 80.f;

	ChildSlot[
		SAssignNew(MainBox, SBox)
		.Visibility(EVisibility::Collapsed)
		[
			SAssignNew(TextBlock, STextBlock)
			.Text(FText::FromString("0"))
			.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
			.Font(Font)
			.Justification(ETextJustify::Center)
		]
	];
}

void SCountdownOverlay::Hide()
{
	if (MainBox.IsValid())
	{
		MainBox->SetVisibility(EVisibility::Collapsed);
	}
}

void SCountdownOverlay::UpdateCountdown(const int32 Seconds)
{
	if (MainBox.IsValid() && TextBlock.IsValid())
	{
		if (Seconds > 0)
		{
			TextBlock->SetText(FText::AsNumber(Seconds));
			MainBox->SetVisibility(EVisibility::Visible);
		}
		else
		{
			Hide();
		}
	}
}
