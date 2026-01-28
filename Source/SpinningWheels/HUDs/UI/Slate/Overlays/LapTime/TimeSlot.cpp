#include "TimeSlot.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void STimeSlot::Construct(const FArguments& InArgs)
{

	const FText Time = InArgs._Time;
	const FLinearColor TimeColor = InArgs._TimeColor;
	
	const FText Diff = InArgs._Diff;
	const FLinearColor DiffColor = InArgs._DiffColor;
	
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
			.ColorAndOpacity(TimeColor)
		]

		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Visibility(Diff.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible)
			.AutoWrapText(true)
			.Font(FontTimesCompare)
			.Text(Diff)
			.ColorAndOpacity(DiffColor)
		]
	];
}
