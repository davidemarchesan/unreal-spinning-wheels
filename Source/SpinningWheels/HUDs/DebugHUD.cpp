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
		const TSharedRef<SDebugOverlay> DebugOverlay = SNew(SDebugOverlay)
			.Car(Car);

		GEngine->GameViewport->AddViewportWidgetContent(DebugOverlay);
	}
}
