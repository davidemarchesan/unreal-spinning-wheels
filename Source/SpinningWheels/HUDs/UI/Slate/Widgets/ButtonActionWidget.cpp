#include "ButtonActionWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SButtonAction::Construct(const FArguments& InArgs)
{
	const FText ButtonText = InArgs._ButtonText;
	const FText ActionText = InArgs._ActionText;

	ChildSlot[
		SNew(SBox)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SBox)
				[
					SNew(SBorder)
					.BorderImage(FMainStyle::Get().GetBrush("Brush.ButtonAction.Button"))
					.Padding(FMargin(7.f, 5.f))
					[
						SNew(STextBlock)
						.Text(ButtonText)
						.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.ButtonAction.Button"))
					]
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(5.f, 1.f))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SBox)
				[
					SNew(STextBlock)
					.Text(ActionText)
					.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.ButtonAction.Action"))
				]
			]
		]
	];
}
