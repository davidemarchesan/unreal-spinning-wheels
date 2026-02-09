#include "TimeSlot.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void STimeSlot::Construct(const FArguments& InArgs)
{
	const FText Time = InArgs._Time;
	const ESlateTimeColor TimeColor = InArgs._TimeColor;

	const FText Diff = InArgs._Diff;
	const ESlateTimeColor DiffColor = InArgs._DiffColor;

	FSlateFontInfo FontTitles = FMainStyle::Get().GetFontStyle("Font.Lato.Regular.p");
	FSlateFontInfo FontTimes = FMainStyle::Get().GetFontStyle("Font.JetBrains.Regular.p");
	FSlateFontInfo FontTimesCompare = FMainStyle::Get().GetFontStyle("Font.JetBrains.Regular.help");

	ChildSlot[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.AutoWrapText(true)
			.Font(FontTimes)
			.Text(Time)
			.ColorAndOpacity(GetColor(TimeColor))
		]

		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Visibility(Diff.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible)
			.AutoWrapText(true)
			.Font(FontTimesCompare)
			.Text(Diff)
			.ColorAndOpacity(GetColor(DiffColor))
		]
	];
}

FLinearColor STimeSlot::GetColor(ESlateTimeColor InColor)
{
	switch (InColor)
	{
	default:
	case ESlateTimeColor::TC_White:
		return FMainStyle::Get().GetColor("Color.Text.Light.Primary");
		break;

	case ESlateTimeColor::TC_Purple:
		return FMainStyle::Get().GetColor("Color.Time.Purple");
		break;

	case ESlateTimeColor::TC_Green:
		return FMainStyle::Get().GetColor("Color.Time.Green");
		break;

	case ESlateTimeColor::TC_Red:
	return FMainStyle::Get().GetColor("Color.Time.Red");
		break;
	}
}
