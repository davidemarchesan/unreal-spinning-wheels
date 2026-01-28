#include "ServerMessagesOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SServerMessagesOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SAssignNew(MainOverlay, SOverlay)
		.Visibility(EVisibility::Collapsed)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.Padding(0.f, 150.f)
		[
			SNew(SBox)
			.WidthOverride(400.f)
			[
				SNew(SBorder)
				.Padding(10.f, 5.f)
				.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Light"))
				[
					SAssignNew(TextBlock, STextBlock)
					.Text(FText::FromString("Starting soon..."))
					.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Dark"))
					.Font(FMainStyle::Get().GetFontStyle("Font.Lato.Regular.p"))
					.Justification(ETextJustify::Center)
				]
			]
		]
	];
}

void SServerMessagesOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
	const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (MainOverlay.IsValid() == false)
	{
		return;
	}
	
	if (MainOverlay->GetVisibility() == EVisibility::Collapsed)
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
	if (MainOverlay.IsValid() == false || TextBlock.IsValid() == false)
	{
		return;
	}

	TextBlock->SetText(NewText);
	MainOverlay->SetVisibility(EVisibility::Visible);

	bTemporary = Seconds > 0.f;
	HideTime = FSlateApplication::Get().GetCurrentTime() + Seconds;
}

void SServerMessagesOverlay::Hide()
{
	if (MainOverlay.IsValid())
	{
		MainOverlay->SetVisibility(EVisibility::Collapsed);;
	}
}
