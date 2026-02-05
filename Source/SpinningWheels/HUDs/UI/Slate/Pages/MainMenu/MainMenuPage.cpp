#include "MainMenuPage.h"

#include "MainMenuItemWidget.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SMainMenuPage::Construct(const FArguments& InArgs)
{

	ChildSlot[
		SNew(SBox)
		.Padding(FMainStyle::Get().GetMargin("Padding.SafeArea"))
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Game.Title"))
				.Text(FText::FromString("Spinning Wheels"))
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size(FVector2D(1.0f, 20.f))
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.f)
			[
				// Actual menu
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SMainMenuItemWidget)
					.Text(FText::FromString("Play"))
					.Page(EMenuPage::MP_Play)
					.OnMainMenuItemSelected(InArgs._OnMainMenuItemSelected)
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SMainMenuItemWidget)
					.Text(FText::FromString("Tracks"))
					.Page(EMenuPage::MP_Tracks)
					.OnMainMenuItemSelected(InArgs._OnMainMenuItemSelected)
				]
				
			]
		]

	];
}

bool SMainMenuPage::SupportsKeyboardFocus() const
{
	return true;
}
