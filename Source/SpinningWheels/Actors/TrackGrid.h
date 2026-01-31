// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackGrid.generated.h"

UENUM()
enum class ETileStatus : uint8
{
	TS_Free,
	TS_Busy
};

class UBoxComponent;

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

protected:
	virtual void BeginPlay() override;

public:	

	void Initialize(int32 InCols, int32 InRows);

};
