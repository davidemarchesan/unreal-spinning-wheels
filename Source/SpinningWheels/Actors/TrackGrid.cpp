// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackGrid.h"

#include "Components/BoxComponent.h"

ATrackGrid::ATrackGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	if (BoxComponent)
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

void ATrackGrid::Initialize(int32 InCols, int32 InRows)
{
	Cols = InCols;
	Rows = InRows;

	InitializeLogicGrid();
	InitializeCollisionGrid();
}

void ATrackGrid::InitializeLogicGrid()
{

	// Grid Y = Rows
	// Grid X = Cols

	// World X = Grid Y
	// World Y = Grid X
	
	Grid.SetNum(Cols);
		
	for (int32 X = 0; X < Cols; X++)
	{
		Grid[X].SetNum(Rows);
		for (int32 Y = 0; Y < Rows; Y++)
		{
			Grid[X][Y] = ETileStatus::TS_Free;
		}
	}
}

void ATrackGrid::InitializeCollisionGrid()
{

	// World X = Grid Y
	// World Y = Grid X
	const float GridXs = Cols * TileSize * 0.5f;
	const float GridYs = Rows * TileSize * 0.5f;

	WorldCenter = FVector (GridYs, GridXs, 0.f);

	SetActorLocation(WorldCenter);
	
	if (BoxComponent)
	{
		const float RadiiTileSize = TileSize * 0.5f;
		BoxComponent->SetBoxExtent(FVector(Rows * RadiiTileSize, Cols * RadiiTileSize, 1.0f));
	}
}

