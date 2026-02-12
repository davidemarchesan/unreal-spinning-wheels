#include "CarStatusOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SCarStatusOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[SNew(SBox).MinDesiredWidth(300.f).MinDesiredHeight(10.f)];
}

void SCarStatusOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (Car.IsValid() == false)
	{
		return;
	}

	if (TurboValue == Car->GetTurboCurrentBattery())
	{

		if (bValueChanged == true)
		{
			OpacityAnimStart = InCurrentTime + OpacityAnimDelay;
		}

		bValueChanged = false;
		
	}
	else
	{
		bValueChanged = true;

		OpacityAnimStart = 0.f;

		TurboValue = Car->GetTurboCurrentBattery();
		CurrentOpacity = 1.f;
	}

	if (bValueChanged == false && CurrentOpacity > 0.f && OpacityAnimStart > 0.f && InCurrentTime >= OpacityAnimStart)
	{
		const float Opacity = FMath::FInterpTo(CurrentOpacity, 0.f, InDeltaTime, OpacityAnimSpeed);
		CurrentOpacity = Opacity;

		if (FMath::IsNearlyZero(CurrentOpacity, 0.01f))
		{
			CurrentOpacity = 0.f;
		}
	}
}

int32 SCarStatusOverlay::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                 const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                 int32 LayerId,
                                 const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (BarSlateBush == nullptr)
	{
		return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
		                                bParentEnabled);
	}

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(TurboBarWidth, TurboBarHeight),
			FSlateLayoutTransform(FVector2D(0.f, 0.f))
		),
		BarSlateBush,
		ESlateDrawEffect::None,
		FMainStyle::Get().GetColor("Color.Text.Dark.Secondary").CopyWithNewOpacity(CurrentOpacity)
	);

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId + 1,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(TurboBarWidth * TurboValue, TurboBarHeight),
			FSlateLayoutTransform(FVector2D(0.f, 0.f))
		),
		BarSlateBush,
		ESlateDrawEffect::None,
		FMainStyle::Get().GetColor("Color.Yellow.Primary").CopyWithNewOpacity(CurrentOpacity)
	);

	return LayerId + 2;
}

void SCarStatusOverlay::SetCar(const TWeakObjectPtr<ACar> InCar)
{
	if (InCar.IsValid())
	{
		Car = InCar;
	}
}
