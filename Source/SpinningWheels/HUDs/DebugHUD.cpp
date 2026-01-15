// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugHUD.h"

#include "SpinningWheels/Pawns/Car.h"
#include "UI/Slate/Overlays/Debug/DebugOverlay.h"

void ADebugHUD::BeginPlay()
{
	Super::BeginPlay();

	Car = Cast<ACar>(GetOwningPawn());

	if (Car.IsValid())
	{
		if (UCarMovementComponent* CarMovementComponent = Car->GetCarMovementComponent())
		{
			const TSharedRef<SDebugOverlay> DebugOverlay = SNew(SDebugOverlay)
				.CarMovementComponent(CarMovementComponent);

			GEngine->GameViewport->AddViewportWidgetContent(DebugOverlay);
		}
	}
	
}
