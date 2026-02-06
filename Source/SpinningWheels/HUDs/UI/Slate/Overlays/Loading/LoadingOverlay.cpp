#include "LoadingOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SLoadingOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[

			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Game.Title"))
				.Text(FText::FromString("Spinning Wheels"))
				.Justification(ETextJustify::Center)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size(FVector2D(1.f, 20.f))
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.P"))
				.Text(FText::FromString("Loading..."))
				.Justification(ETextJustify::Center)
			]
			
		]
	];
}
