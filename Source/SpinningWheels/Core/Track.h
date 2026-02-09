#pragma once
// #include "SpinningWheels/Actors/TrackGrid.h"

#include "Track.generated.h"

namespace TrackConstants
{
	constexpr int32 Size = 100;
}

USTRUCT()
struct FGridCoord
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 X = 0;

	UPROPERTY()
	int32 Y = 0;

	UPROPERTY()
	int32 R = 0;

public:

	FORCEINLINE bool operator==(const FGridCoord& Other) const
	{
		return (X == Other.X)
			&& (Y == Other.Y);
	}

public:

	FGridCoord() = default;

	FGridCoord(int32 InX, int32 InY)
		: X(InX), Y(InY)
	{
	}

	FGridCoord(int32 InX, int32 InY, int32 InR)
		: X(InX), Y(InY), R(InR)
	{
	}

	FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("X:%d Y:%d R:%d"), X, Y, R);
	};
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
