#include "ServerMessagesOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SServerMessagesOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[

		SAssignNew(MainBox, SBox)
		.Visibility(EVisibility::Collapsed)
		.WidthOverride(400.f)
		[
			SNew(SBorder)
			.Padding(10.f, 5.f)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Light"))
			[
				SAssignNew(TextBlock, STextBlock)
				.Text(FText::FromString("Starting soon..."))
				.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.P"))
				.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Dark.Primary"))
				.Justification(ETextJustify::Center)
			]
		]

	];
}

void SServerMessagesOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
                                  const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (MainBox.IsValid() == false)
	{
		return;
	}

	if (MainBox->GetVisibility() == EVisibility::Collapsed)
	{
		return;
	}

	if (bTemporary == false)
	{
		return;
	}

	if (InCurrentTime > HideTime)
	{
		Hide();
	}
}

void SServerMessagesOverlay::Show(const FText& NewText, float Seconds)
{
	if (MainBox.IsValid() == false || TextBlock.IsValid() == false)
	{
		return;
	}

	TextBlock->SetText(NewText);
	MainBox->SetVisibility(EVisibility::Visible);

	bTemporary = Seconds > 0.f;
	HideTime = FSlateApplication::Get().GetCurrentTime() + Seconds;
}

void SServerMessagesOverlay::Hide()
{
	if (MainBox.IsValid())
	{
		MainBox->SetVisibility(EVisibility::Collapsed);;
	}
}
