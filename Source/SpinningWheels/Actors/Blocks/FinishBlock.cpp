// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishBlock.h"

#include "Components/BoxComponent.h"

AFinishBlock::AFinishBlock()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	if (BoxComponent)
	{
		BoxComponent->SetupAttachment(RootComponent);
	}
}
