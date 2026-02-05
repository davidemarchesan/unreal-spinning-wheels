#include "ModalBaseWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SModalBase::Construct(const FArguments& InArgs)
{

	ChildSlot[

		SAssignNew(MainBox, SBox)
		.MinDesiredWidth(300.f)
		[
			SNew(SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
			.Padding(FMainStyle::Get().GetMargin("Padding.Box"))
			[
				SNew(SVerticalBox)
				
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					InArgs
					._HeaderSlot
					.Widget
				]
				
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					InArgs
					._BodySlot
					.Widget
				]
				
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					InArgs
					._FooterSlot
					.Widget
				]

			]
		]
	];
}
