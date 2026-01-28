#include "CountdownOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SCountdownOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo Font = FMainStyle::Get().GetFontStyle("Font.JetBrains.Bold.h1");
	Font.Size = 80.f;

	ChildSlot[
		SAssignNew(MainOverlay, SOverlay)
		.Visibility(EVisibility::Collapsed)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			[
				SAssignNew(TextBlock, STextBlock)
				.Text(FText::FromString("0"))
				.ColorAndOpacity(FMainStyle::Get().GetColor("Color.CharcoalBlue"))
				.Font(Font)
				.Justification(ETextJustify::Center)
			]
		]
	];
}

void SCountdownOverlay::Hide()
{
	MainOverlay->SetVisibility(EVisibility::Collapsed);
}

void SCountdownOverlay::UpdateCountdown(const int32 Seconds)
{
	if (MainOverlay.IsValid() && TextBlock.IsValid())
	{
		if (Seconds > 0)
		{
			TextBlock->SetText(FText::AsNumber(Seconds));
			MainOverlay->SetVisibility(EVisibility::Visible);
		}
		else
		{
			Hide();
		}
	}
}