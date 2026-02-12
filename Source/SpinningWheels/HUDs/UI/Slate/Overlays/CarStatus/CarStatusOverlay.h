#pragma once
#include "SpinningWheels/Pawns/Car.h"

class SCarStatusOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCarStatusOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
private:

	float TurboBarWidth = 300.f;
	float TurboBarHeight = 10.f;

	float OpacityAnimDelay = 2.f;		// How many seconds to wait to start fading out
	float OpacityAnimSpeed = 4.f;
	float OpacityAnimStart = 0.f;

	float TurboValue = 1.f;
	float CurrentOpacity = 0.f;

	bool bValueChanged = false;
	
	TWeakObjectPtr<ACar> Car;

	FSlateBrush* BarSlateBush = new FSlateRoundedBoxBrush(FLinearColor::Transparent, 6.f, FLinearColor::Transparent, 0.f);

public:

	void SetCar(const TWeakObjectPtr<ACar> InCar);
	
};
