#pragma once

#include "Track.generated.h"

USTRUCT()
struct FTrack
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Id;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 Sectors = 0;

public:
	FTrack() = default;

	FTrack(const FString& InId, const FString& InName, const int32 InSectors)
		: Id(InId),
		  Name(InName),
		  Sectors(InSectors)
	{
	}
};
