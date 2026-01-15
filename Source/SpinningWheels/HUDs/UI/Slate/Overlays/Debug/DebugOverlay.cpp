#include "DebugOverlay.h"

#include "SpinningWheels/Actors/Components/CarMovementComponent.h"

void SDebugOverlay::Construct(const FArguments& InArgs)
{
	CarMovementComponent = InArgs._CarMovementComponent;

	ChildSlot[

		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(10.f)
		[
			SNew(SBox)
			.WidthOverride(300.f)
			[
				SNew(SBorder)
				.Padding(10.f)
				.BorderImage(new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.5f), 4.f))
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.Text(FText::FromString("Debug Overlay"))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SAssignNew(DebugText, STextBlock)
						.AutoWrapText(true)
					]
				]
			]
		]


	];
}

void SDebugOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (CarMovementComponent.IsValid())
	{
		if (DebugText.IsValid())
		{
			DebugText->SetText(FText::Format(
				FText::FromString("Velocity: {0}\nVelocity Length: {1}\nCurrent Acceleration: {2}"),
				FText::FromString(CarMovementComponent->GetVelocity().ToString()),
				FText::AsNumber(CarMovementComponent->GetVelocity().Length()),
				FText::AsNumber(CarMovementComponent->GetCurrentAcceleration())
			));
		}
	}
}
