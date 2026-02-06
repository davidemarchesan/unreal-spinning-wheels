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
			SNew(SBox)
			.Padding(FMainStyle::Get().GetMargin("Padding.SafeArea"))
			[
				InArgs
				._BodySlot
				.Widget
			]
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
				.AutoWidth()
				[

					SNew(SButtonAction)
					.ButtonText(FText::FromString("Esc"))
					.ActionText(FText::FromString("Back"))

				]

				// Context actions (or right actions)
				+ SHorizontalBox::Slot()
				.FillContentWidth(1.f)
				.HAlign(HAlign_Right)
				[
					InArgs._ContextActionsSlot.Widget
				]

			]
		]

	];
}
