#include "InfoOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SInfoOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SNew(SBox)
		[
			SAssignNew(PlayerIdTextBlock, STextBlock)
			.Text(FText::GetEmpty())
			.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Dark.Primary"))
			.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.Regular.p"))
			.Justification(ETextJustify::Left)
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
