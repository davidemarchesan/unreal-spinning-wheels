#include "LapTimeOverlay.h"

#include "TimeSlot.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SLapTimeOverlay::Construct(const FArguments& InArgs)
{
	const FSlateBrush* Background = FMainStyle::Get().GetBrush("Brush.Background.Black");

	FSlateFontInfo FontTitles = FMainStyle::Get().GetFontStyle("Font.Lato.Regular.p");
	FSlateFontInfo FontTimes = FMainStyle::Get().GetFontStyle("Font.JetBrains.Regular.p");
	FSlateFontInfo FontTimesCompare = FMainStyle::Get().GetFontStyle("Font.JetBrains.Regular.help");

	float ColumnWidth = 200.f;

	ChildSlot[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		.Padding(10.f)
		[

			SNew(SBorder)
			.BorderImage(Background)
			[

				SNew(SBox)
				.Padding(10.f, 6.f)
				[

					SNew(SVerticalBox)

					// Current Match Record
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox)
							.MinDesiredWidth(ColumnWidth)
							[
								SNew(STextBlock)
								.AutoWrapText(true)
								.Text(FText::FromString("Pers. Best"))
								.Font(FontTitles)
								.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Light"))
							]
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox)
							.MinDesiredWidth(ColumnWidth)
							[
								SNew(STimeSlot)
								.Time(FText::FromString("---"))
								.TimeColor(FMainStyle::Get().GetColor("Color.Primary.Light"))
							]
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox)
							.MinDesiredWidth(ColumnWidth)
							[
								SNew(STimeSlot)
								.Time(FText::FromString("---"))
								.TimeColor(FMainStyle::Get().GetColor("Color.Primary.Light"))
							]
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox)
							.MinDesiredWidth(ColumnWidth)
							[
								SNew(STimeSlot)
								.Time(FText::FromString("---"))
								.TimeColor(FMainStyle::Get().GetColor("Color.Primary.Light"))
							]
						]
					]

				]

			]

		]
	];
}
