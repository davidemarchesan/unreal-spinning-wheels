#include "TimeNameSlot.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void STimeNameSlot::Construct(const FArguments& InArgs)
{
	const FText Title = InArgs._Title;

	FSlateFontInfo FontTitles = FMainStyle::Get().GetFontStyle("Font.Lato.Regular.p");
	FSlateFontInfo FontTimes = FMainStyle::Get().GetFontStyle("Font.JetBrains.Regular.p");
	FSlateFontInfo FontTimesCompare = FMainStyle::Get().GetFontStyle("Font.JetBrains.Regular.help");

	ChildSlot[
		SNew(STextBlock)
		.AutoWrapText(true)
		.Text(Title)
		.Font(FontTitles)
		.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Light"))

	];
}
