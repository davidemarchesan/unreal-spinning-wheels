// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackGrid.h"

#include "Blocks/Block.h"
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

FGridCoord ATrackGrid::GetTileCoordinates(FVector WorldLocation)
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

FVector ATrackGrid::GetTileWorldLocation(const FGridCoord& Coordinates)
{
	const int32 X = Coordinates.X * TileSize + (TileSize * 0.5f);
	const int32 Y = Coordinates.Y * TileSize + (TileSize * 0.5f);
	
	return FVector(Y, X, 0.f);
}

bool ATrackGrid::CanBuildOn(FVector WorldLocation)
{

	FGridCoord Coordinates = GetTileCoordinates(WorldLocation);
	
	if (Grid.IsValidIndex(Coordinates.X) == false)
	{
		return false;
	}

	if (Grid[Coordinates.X].IsValidIndex(Coordinates.Y) == false)
	{
		return false;
	}
	
	return Grid[Coordinates.X][Coordinates.Y] != ETileStatus::TS_Busy;
	
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

void ATrackGrid::Initialize(int32 InCols, int32 InRows, const FTrackSaveData& TrackSaveData)
{
	Initialize(InCols, InRows);

	TArray<FTrackBlockSaveData> BlockSaveData = TrackSaveData.Blocks;

	for (const FTrackBlockSaveData& Block : BlockSaveData)
	{
		Build(Block.BlockId, Block.Coordinates);
	}
}

void ATrackGrid::Build(const FName& BlockName, FVector WorldLocation, FRotator Rotation)
{
	const FGridCoord GridCoord = GetTileCoordinates(WorldLocation);

	// Avoid to build on an already occupied tile
	if (Grid[GridCoord.X][GridCoord.Y] == ETileStatus::TS_Busy)
	{
		return;
	}

	Build(BlockName, GridCoord);
	
}

void ATrackGrid::Build(const FName& BlockName, const FGridCoord& Coordinates)
{
	if (BlocksTable == nullptr)
	{
		return;
	}

	if (FBlockRow* BlockRow = BlocksTable->FindRow<FBlockRow>(BlockName, "Build"))
	{
		TSubclassOf<ABlock> BlockClass = BlockRow->BlockClass;
		if (UWorld* World = GetWorld())
		{
			FVector Location = GetTileWorldLocation(Coordinates);
			ABlock* NewBlock = World->SpawnActor<ABlock>(BlockClass, Location, FRotator::ZeroRotator);

			Grid[Coordinates.X][Coordinates.Y] = ETileStatus::TS_Busy;

			FTrackBlock NewTrackBlock(BlockName, Coordinates);
			Blocks.Add(NewTrackBlock);
		}
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
	const float CenterX = Cols * TileSize * 0.5f;
	const float CenterY = Rows * TileSize * 0.5f;

	WorldCenter = FVector (CenterY, CenterX, 0.f);

	SetActorLocation(WorldCenter);
	
	if (BoxComponent)
	{
		const float RadiiTileSize = TileSize * 0.5f;
		BoxComponent->SetBoxExtent(FVector(Rows * RadiiTileSize, Cols * RadiiTileSize, 1.0f));
	}
}

