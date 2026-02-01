// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackGrid.h"

#include "Blocks/Block.h"
#include "Components/BoxComponent.h"
#include "SpinningWheels/Core/Grid.h"

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

FGridCoord ATrackGrid::GetTileCoord(FVector WorldLocation)
{
	const int32 X = FMath::FloorToInt32(WorldLocation.Y / TileSize);
	const int32 Y = FMath::FloorToInt32(WorldLocation.X / TileSize);

	return FGridCoord(X, Y);
}

FVector ATrackGrid::GetTileWorldLocation(FVector WorldLocation)
{
	const int32 X = FMath::FloorToInt32(WorldLocation.Y / TileSize) * TileSize + (TileSize * 0.5f);
	const int32 Y = FMath::FloorToInt32(WorldLocation.X / TileSize) * TileSize + (TileSize * 0.5f);

	return FVector(Y, X, 0.f);
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

void ATrackGrid::Build(TSubclassOf<ABlock> BlockClass, FVector WorldLocation, FRotator Rotation)
{
	if (UWorld* World = GetWorld())
	{
		FGridCoord GridCoord = GetTileCoord(WorldLocation);

		// Avoid to build on an already occupied tile
		if (Grid[GridCoord.X][GridCoord.Y] == ETileStatus::TS_Busy)
		{
			return;
		}
		
		FVector Location = GetTileWorldLocation(WorldLocation);
		ABlock* NewBlock = World->SpawnActor<ABlock>(BlockClass, Location, Rotation);

		Grid[GridCoord.X][GridCoord.Y] = ETileStatus::TS_Busy;
	}
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

