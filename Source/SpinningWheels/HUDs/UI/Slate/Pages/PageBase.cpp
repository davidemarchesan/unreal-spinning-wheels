#include "PageBase.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/ButtonActionWidget.h"

void SPageBase::Construct(const FArguments& InArgs)
{
	ChildSlot[

		SNew(SVerticalBox)

		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.Padding(FMainStyle::Get().GetMargin("Padding.SafeArea"))
			[
				SNew(STextBlock)
				.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Page.Title"))
				.Text(InArgs._PageTitleText)
			]
		]

		// Body
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			InArgs._BodySlot.Widget
		]

		// Footer
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.Padding(FMainStyle::Get().GetMargin("Padding.SafeArea"))
			[

				SNew(SHorizontalBox)

				// Left actions
				+ SHorizontalBox::Slot()
				[
					
					SNew(SButtonAction)
					.ButtonText(FText::FromString("Esc"))
					.ActionText(FText::FromString("Back"))

				]

				// Right actions
				+ SHorizontalBox::Slot()


			]
		]

	];
}
