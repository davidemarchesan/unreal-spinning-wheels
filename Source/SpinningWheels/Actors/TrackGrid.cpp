// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackGrid.h"

#include "Components/BoxComponent.h"

ATrackGrid::ATrackGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	if (BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box"))
	{
		RootComponent = BoxComponent;
	}

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
	}
	
}

void ATrackGrid::BeginPlay()
{
	Super::BeginPlay();
}

void ATrackGrid::Initialize(int32 InRows, int32 InCols)
{
	Rows = InRows;
	Cols = InCols;
}

