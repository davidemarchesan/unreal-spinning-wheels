// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpinningWheels/Core/Track.h"
#include "TrackGrid.generated.h"

class UBoxComponent;
class ABlock;

UENUM()
enum class ETileStatus : uint8
{
	TS_Free,
	TS_Busy
};

UCLASS()
class SPINNINGWHEELS_API ATrackGrid : public AActor
{
	GENERATED_BODY()

public:
	ATrackGrid();

private:
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	int32 Rows = 0;
	int32 Cols = 0;

	int32 TileSize = 1500;

	TArray<TArray<ETileStatus>> Grid;
	TArray<FTrackBlock> Blocks;

	void InitializeLogicGrid();
	void InitializeCollisionGrid();

	FVector WorldCenter;

	FGridCoord GetTileCoord(FVector WorldLocation);
	FVector GetTileWorldLocation(FVector WorldLocation);
	FVector GetTileWorldLocation(const FGridCoord& Coordinates);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category=Data, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDataTable> BlocksTable;

public:
	void Initialize(int32 InCols, int32 InRows);
	void Initialize(int32 InCols, int32 InRows, const FTrackSaveData& TrackSaveData);

	void Build(const FName& BlockName, FVector WorldLocation, FRotator Rotation);
	void Build(const FName& BlockName, const FGridCoord& Coordinates);

	TArray<FTrackBlock> GetBlocks() const {	return Blocks; }
};
