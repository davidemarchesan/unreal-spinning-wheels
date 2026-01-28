#include "InfoOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SInfoOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SAssignNew(MainOverlay, SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		[
			SNew(SBox)
			[
				SAssignNew(PlayerIdTextBlock, STextBlock)
				.Text(FText::GetEmpty())
				.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Dark"))
				.Font(FMainStyle::Get().GetFontStyle("Font.Lato.Regular.p"))
				.Justification(ETextJustify::Left)
			]
		]
	];
}

void SInfoOverlay::SetPlayerId(int32 PlayerId)
{
	if (PlayerIdTextBlock.IsValid())
	{
		PlayerIdTextBlock->SetText(FText::AsNumber(PlayerId));
	}
}
