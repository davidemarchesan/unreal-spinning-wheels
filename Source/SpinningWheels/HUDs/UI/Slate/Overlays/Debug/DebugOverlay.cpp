#include "DebugOverlay.h"

#include "Widgets/SCanvas.h"

void SDebugOverlay::Construct(const FArguments& InArgs)
{
	Car = InArgs._Car;
	if (Car.IsValid())
	{
		CarMovementComponent = Car->GetCarMovementComponent();
	}

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

			// Stats
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
						SAssignNew(StatsTextBlock, STextBlock)
						.AutoWrapText(true)
					]
				]
			]

			// Velocity graph
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
							SNew(STextBlock)
							.Text(FText::FromString("Velocity"))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBox)
							.WidthOverride(BoxWidth)
							.HeightOverride(200.f)
							[
								SNew(SCanvas)

								+ SCanvas::Slot()
								.Position(FVector2D(280.f * 0.5f, 100.0f))
								.Size(FVector2D(100.f, 2.f))
								[
									SAssignNew(Velocity2DViewer, SBorder)
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

			// Facing direction graph
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
							SNew(STextBlock)
							.Text(FText::FromString("Facing Direction"))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBox)
							.WidthOverride(BoxWidth)
							.HeightOverride(200.f)
							[
								SNew(SCanvas)

								+ SCanvas::Slot()
								.Position(FVector2D(280.f * 0.5f, 100.0f))
								.Size(FVector2D(100.f, 2.f))
								[
									SAssignNew(FacingDirection2DViewer, SBorder)
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

	if (Car.IsValid() == false || CarMovementComponent.IsValid() == false)
	{
		return;
	}


	if (StatsTextBlock.IsValid())
	{
		StatsTextBlock->SetText(FText::Format(
			FText::FromString("Mode: {0}\nVelocity Length: {1}\n Acceleration: {2}\nBrake Deceleration: {3}\nAngular Velocity: {4}\nTurbo: {5}"),
			CarMovementComponent->GetCarMode(),
			FText::AsNumber(CarMovementComponent->GetVelocity().Length()),
			FText::AsNumber(CarMovementComponent->GetAcceleration()),
			FText::AsNumber(CarMovementComponent->GetBrakeDeceleration()),
			FText::FromString(CarMovementComponent->GetAngularVelocity().ToString()),
			FText::AsNumber(Car->GetTurboCurrentBattery())
		));
	}

	if (Velocity2DViewer.IsValid())
	{
		FVector Velocity = CarMovementComponent->GetVelocity().GetSafeNormal();
		FVector2D Velocity2D = FVector2D(Velocity.X, Velocity.Y);

		Velocity2DViewer->SetRenderTransform(
			FSlateRenderTransform(
				FQuat2D(
					FMath::Atan2(-Velocity2D.X, Velocity2D.Y)
				)
			)
		);
	}

	if (FacingDirection2DViewer.IsValid())
	{
		FVector FacingDirection = Car->GetActorForwardVector();
		FVector2D FacingDirection2D = FVector2D(FacingDirection.X, FacingDirection.Y);

		FacingDirection2DViewer->SetRenderTransform(
			FSlateRenderTransform(
				FQuat2D(
					FMath::Atan2(-FacingDirection2D.X, FacingDirection2D.Y)
				)
			)
		);
	}
}
