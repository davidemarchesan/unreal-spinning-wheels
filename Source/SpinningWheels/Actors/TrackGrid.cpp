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

bool ATrackGrid::AreValidCoordinates(const FGridCoord& CoordinatesToCheck)
{
	if (Grid.IsValidIndex(CoordinatesToCheck.X) == false)
	{
		return false;
	}

	if (Grid[CoordinatesToCheck.X].IsValidIndex(CoordinatesToCheck.Y) == false)
	{
		return false;
	}

	return true;
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

	if (AreValidCoordinates(Coordinates) == false)
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
	FGridCoord Coordinates = GetTileCoordinates(WorldLocation);
	Coordinates.R = FMath::RoundToInt32(Rotation.Yaw);

	if (AreValidCoordinates(Coordinates) == false)
	{
		return;
	}
	
	// Avoid to build on an already occupied tile
	if (Grid[Coordinates.X][Coordinates.Y] == ETileStatus::TS_Busy)
	{
		return;
	}

	Build(BlockName, Coordinates);
	
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
			FRotator Rotation = FRotator(0.f, Coordinates.R, 0.f);
			ABlock* NewBlock = World->SpawnActor<ABlock>(BlockClass, Location, Rotation);

			Grid[Coordinates.X][Coordinates.Y] = ETileStatus::TS_Busy;

			const FTrackBlock NewTrackBlock(BlockName, Coordinates);
			Blocks.Add(NewTrackBlock);
		}
	}
}

bool ATrackGrid::Remove(FVector WorldLocation)
{
	FGridCoord Coordinates = GetTileCoordinates(WorldLocation);

	if (AreValidCoordinates(Coordinates) == false)
	{
		return false;
	}

	if (Grid[Coordinates.X][Coordinates.Y] == ETileStatus::TS_Free)
	{
		return false;
	}

	const int32 Index = Blocks.IndexOfByPredicate([Coordinates](const FTrackBlock& Block)
	{
		return Block.Coordinates == Coordinates;
	});

	if (Index == INDEX_NONE || Blocks.IsValidIndex(Index) == false)
	{
		return false;
	}

	Blocks.RemoveAt(Index);
	Grid[Coordinates.X][Coordinates.Y] = ETileStatus::TS_Free;

	return true;
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

