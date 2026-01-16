#pragma once

#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/Pawns/Components/CarMovementComponent.h"
#include "Widgets/SCompoundWidget.h"


class SDebugOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SDebugOverlay) {}
		SLATE_ARGUMENT(TWeakObjectPtr<ACar>, Car)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	TWeakObjectPtr<ACar> Car;
	TWeakObjectPtr<UCarMovementComponent> CarMovementComponent;

	TSharedPtr<STextBlock> StatsTextBlock;
	TSharedPtr<SBorder> Velocity2DViewer;
	TSharedPtr<SBorder> FacingDirection2DViewer;
	
};
