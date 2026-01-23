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

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->ArrowSize = 0.5f;
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(StartSceneComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif
	
}

void AStartBlock::GetStartingPoint(FVector& OutLocation, FRotator& OutDirection)
{
	if (StartSceneComponent)
	{
		OutLocation = StartSceneComponent->GetComponentLocation();
		OutDirection = StartSceneComponent->GetComponentRotation();
	}
}
