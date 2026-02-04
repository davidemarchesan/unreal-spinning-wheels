// Fill out your copyright notice in the Description page of Project Settings.


#include "StartBlock.h"

#include "Components/ArrowComponent.h"

AStartBlock::AStartBlock()
{
	StartSceneComponent = CreateDefaultSubobject<USceneComponent>("Start");
	if (StartSceneComponent)
	{
		StartSceneComponent->SetupAttachment(RootComponent);
	}

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->ArrowSize = 12.f;
		ArrowComponent->ArrowLength = 42.f;
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(StartSceneComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
		ArrowComponent->SetHiddenInGame(false);
	}
	
}

void AStartBlock::GetStartingPoint(FVector& OutLocation, FRotator& OutDirection)
{
	if (StartSceneComponent)
	{
		OutLocation = StartSceneComponent->GetComponentLocation();
		OutDirection = StartSceneComponent->GetComponentRotation();
	}
}
