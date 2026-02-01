// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpinningWheels/Core/Grid.h"
#include "TrackGrid.generated.h"

class UBoxComponent;
class ABlock;

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

	void InitializeLogicGrid();
	void InitializeCollisionGrid();

	FVector WorldCenter;

	FGridCoord GetTileCoord(FVector WorldLocation);
	FVector GetTileWorldLocation(FVector WorldLocation);

protected:
	virtual void BeginPlay() override;

public:	

	void Initialize(int32 InCols, int32 InRows);

	void Build(TSubclassOf<ABlock> BlockClass, FVector WorldLocation, FRotator Rotation);

};
