// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointBlock.h"

#include "Components/BoxComponent.h"

ACheckpointBlock::ACheckpointBlock()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	if (BoxComponent)
	{
		BoxComponent->SetupAttachment(RootComponent);
	}
}
