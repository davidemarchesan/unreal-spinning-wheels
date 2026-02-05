#include "ModalConfirmWidget.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SModalConfirm::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SNew(SModalBase)
		.BodySlot()
		[
			SNew(SBox)
			.Padding(0.f, 0.f, 0.f, 10.f)
			[
				SNew(STextBlock)
				.Text(InArgs._Text)
				.Justification(ETextJustify::Center)
				.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.P"))

			]
		]
		.FooterSlot()
		[
			SNew(SButton)
			.Text(FText::FromString("Confirm"))
			.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Primary"))
			.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Button.Primary"))
			.HAlign(HAlign_Center)
			.OnClicked(InArgs._OnConfirm)
		]
	];
}
