#include "EditorTrackDataOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorTrackDataOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SNew(SBox)
		[
			SNew(SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
			.Padding(FMainStyle::Get().GetMargin("Padding.Box"))
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(IdTextBlock, STextBlock)
					.Text(FText::FromString("Track ID: -"))
					.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.P"))
				]

				+ SVerticalBox::Slot()
				[
					SNew(SSpacer)
					.Size(FVector2D(1.f, 5.f))
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(NameTextBlock, STextBlock)
					.Text(FText::FromString("Track Name: -"))
					.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.P"))
				]
				
			]
		]
	];
}

void SEditorTrackDataOverlay::Update(const FTrack& CurrentTrack)
{
	if (IdTextBlock.IsValid())
	{
		IdTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Track ID: %s"), *CurrentTrack.Id.ToString())));
	}
	if (NameTextBlock.IsValid())
	{
		NameTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Track Name: %s"), *CurrentTrack.Name)));
	}
}
