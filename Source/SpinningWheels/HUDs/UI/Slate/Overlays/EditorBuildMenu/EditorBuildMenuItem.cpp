#include "EditorBuildMenuItem.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorBuildMenuItem::Construct(const FArguments& InArgs)
{
	const int8 Slot = InArgs._Slot;
	const FText Name = InArgs._Name;

	ButtonStyle = FButtonStyle();
	ButtonStyle.SetNormal(FSlateRoundedBoxBrush(FLinearColor::Transparent, 6.f));
	ButtonStyle.SetHovered(FSlateRoundedBoxBrush(FLinearColor::Transparent, 6.f));
	ButtonStyle.SetPressed(FSlateRoundedBoxBrush(FLinearColor::Transparent, 6.f));
	// ButtonStyle.SetHovered(FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.PacificBlue"), 6.f));
	// ButtonStyle.SetPressed(FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.AlabasterGrey"), 6.f));

	ChildSlot[

		SNew(SBox)
		.HeightOverride(50.f)
		.WidthOverride(50.f)
		[
			SNew(SButton)
			.ButtonStyle(&ButtonStyle)
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(FMainStyle::Get().GetFontStyle("Font.Lato.Bold.p"))
					.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Light"))
					.Justification(ETextJustify::Center)
					.Text(FText::AsNumber(Slot))
				]

				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(FMainStyle::Get().GetFontStyle("Font.Lato.Bold.p"))
					.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Light"))
					.Justification(ETextJustify::Center)
					.Text(Name)
				]
			]
		]

	];
}

void SEditorBuildMenuItem::SetSelected()
{
	ButtonStyle.SetNormal(FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.PacificBlue"), 6.f));
}

void SEditorBuildMenuItem::SetUnselected()
{
	ButtonStyle.SetNormal(FSlateRoundedBoxBrush(FLinearColor::Transparent, 6.f));
}
