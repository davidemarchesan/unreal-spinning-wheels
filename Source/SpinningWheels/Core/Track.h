#pragma once
// #include "SpinningWheels/Actors/TrackGrid.h"

#include "Track.generated.h"

USTRUCT()
struct FGridCoord
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 X = 0;

	UPROPERTY()
	int32 Y = 0;

	FGridCoord() = default;

	FGridCoord(int32 InX, int32 InY)
		: X(InX), Y(InY)
	{
	}
};

USTRUCT()
struct FTrackBlock
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName BlockId;

	UPROPERTY()
	FGridCoord Coordinates;

	FTrackBlock() = default;

	FTrackBlock(const FName& InId, const FGridCoord& InGridCoord)
		: BlockId(InId),
		  Coordinates(InGridCoord)
	{
	}
};

USTRUCT()
struct FTrack
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGuid Id;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 Sectors = 0;

public:
	bool IsNull() const
	{
		return Id.IsValid() == false;
	}

public:
	FTrack() = default;

	FTrack(const FGuid& InId, const FString& InName)
		: Id(InId),
		  Name(InName)
	{
	}

	FTrack(const FGuid& InId, const FString& InName, const int32 InSectors)
		: Id(InId),
		  Name(InName),
		  Sectors(InSectors)
	{
	}
};

USTRUCT()
struct FTrackBlockSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName BlockId;

	UPROPERTY()
	FGridCoord Coordinates;

	FTrackBlockSaveData() = default;

	FTrackBlockSaveData(const FName& InId, const FGridCoord& InGridCoord)
		: BlockId(InId),
		  Coordinates(InGridCoord)
	{
	}
};

USTRUCT()
struct FTrackSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Version;

	UPROPERTY()
	FGuid Id;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	TArray<FTrackBlockSaveData> Blocks;
};
