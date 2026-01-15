#pragma once

#include "SpinningWheels/Actors/Components/CarMovementComponent.h"
#include "Widgets/SCompoundWidget.h"


class SDebugOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SDebugOverlay) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UCarMovementComponent>, CarMovementComponent)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	TWeakObjectPtr<UCarMovementComponent> CarMovementComponent;

	TSharedPtr<STextBlock> DebugText;
	
};
