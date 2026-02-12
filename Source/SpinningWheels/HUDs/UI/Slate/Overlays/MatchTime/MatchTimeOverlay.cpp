#include "MatchTimeOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SMatchTimeOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo Font = FMainStyle::Get().GetFontStyle("Font.JetBrains.Bold.h1");
	Font.Size = 45.f;
	Font.OutlineSettings = FFontOutlineSettings(1, FMainStyle::Get().GetColor("Color.Text.Dark.Primary"));

	ChildSlot[
		SAssignNew(TextBlock, STextBlock)
		.Text(FText::FromString("00:00"))
		.Font(Font)
		.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
	];
}

void SMatchTimeOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (RemainingSecondsCeil > 0.f)
	{
		RemainingSeconds -= InDeltaTime;

		const int32 TempRemainingSecondsFloor = FMath::FloorToInt32(RemainingSeconds);
		if (TempRemainingSecondsFloor != RemainingSecondsCeil)
		{
			RemainingSecondsCeil = TempRemainingSecondsFloor;
			UpdateVisuals();
		}
	}
}

void SMatchTimeOverlay::UpdateVisuals()
{

	const int Minutes = FMath::FloorToInt(RemainingSeconds / 60.f);
	const int Seconds = FMath::FloorToInt(RemainingSeconds - (60.f * Minutes));

	FString Str = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	if (TextBlock.IsValid())
	{
		TextBlock->SetText(FText::FromString(Str));
	}
	
}

void SMatchTimeOverlay::SetMatchRemainingTime(const float Seconds)
{
	RemainingSeconds = Seconds;
	RemainingSecondsCeil = FMath::FloorToInt(RemainingSeconds);
	UpdateVisuals();
}
