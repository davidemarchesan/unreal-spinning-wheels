#pragma once

#include "Grid.generated.h"

class ABlock;

struct FGridCoord
{
public:
	
	int32 X = 0;
	int32 Y = 0;

	FGridCoord() = default;
	
	FGridCoord(int32 InX, int32 InY)
		: X(InX), Y(InY)
	{
	}
	
};

UENUM()
enum class ETileStatus : uint8
{
	TS_Free,
	TS_Busy
};

// todo: wip - for saving and loading
USTRUCT()
struct FTileInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int32 X;

	UPROPERTY()
	int32 Y;

	UPROPERTY()
	TSubclassOf<ABlock> BlockClass;
	
};