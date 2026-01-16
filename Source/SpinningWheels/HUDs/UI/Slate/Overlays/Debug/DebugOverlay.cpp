#include "DebugOverlay.h"

// #include "SpinningWheels/Actors/Components/CarMovementComponent.h"
#include "Widgets/SCanvas.h"

void SDebugOverlay::Construct(const FArguments& InArgs)
{
	CarMovementComponent = InArgs._CarMovementComponent;

	const float BoxWidth = 300.f;
	const float BoxPadding = 10.f;
	const FSlateRoundedBoxBrush* BoxBrush = new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.5f), 4.f);

	ChildSlot[

		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(10.f)
		[

			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.Padding(0.f, BoxPadding)
			.AutoHeight()
			[
				SNew(SBox)
				.WidthOverride(BoxWidth)
				[
					SNew(SBorder)
					.Padding(BoxPadding)
					.BorderImage(BoxBrush)
					[
						SAssignNew(DebugText, STextBlock)
						.AutoWrapText(true)
					]
				]
			]

			+ SVerticalBox::Slot()
			.Padding(0.f, BoxPadding)
			.AutoHeight()
			[
				SNew(SBox)
				.WidthOverride(BoxWidth)
				[
					SNew(SBorder)
					.Padding(BoxPadding)
					.BorderImage(BoxBrush)
					[
						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBox)
							.WidthOverride(BoxWidth)
							.HeightOverride(200.f)
							[
								SNew(SCanvas)

								+ SCanvas::Slot()
								.Position(FVector2D(BoxWidth * 0.5f, 100.0f))
								.Size(FVector2D(100.f, 2.f))
								[
									SAssignNew(VelocityBorder, SBorder)
									                                   .BorderImage(
										                                   new FSlateColorBrush(FLinearColor::Red))
									                                   .RenderTransformPivot(FVector2D(0.f, 0.5f))
									                                   .RenderTransform(
										                                   FSlateRenderTransform(
											                                   FQuat2D(FMath::Atan2(-1.f, 1.f))))
								]
							]

						]
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
				FText::FromString("Acceleration: {0}\nSpeed: {1}"),
				FText::AsNumber(CarMovementComponent->GetCurrentAcceleration()),
				FText::AsNumber(CarMovementComponent->GetCurrentSpeed())
			));
		}
	
		// if (VelocityBorder.IsValid())
		// {
		// 	FVector Velocity = CarMovementComponent->GetVelocity().GetSafeNormal();
		// 	FVector2D Velocity2D = FVector2D(Velocity.X, Velocity.Y);
		//
		// 	VelocityBorder->SetRenderTransform(
		// 		FSlateRenderTransform(
		// 			FQuat2D(
		// 				FMath::Atan2(-Velocity2D.X, Velocity2D.Y)
		// 			)
		// 		)
		// 	);
		// }
	}
}
