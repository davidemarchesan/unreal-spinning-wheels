#pragma once
#include "Lap.h"

#include "Leaderboard.generated.h"

USTRUCT()
struct FTimeAttackLeaderboardRow
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY() int8 Position = 0;
	UPROPERTY() int32 PlayerId;
	UPROPERTY() FString PlayerName;
	UPROPERTY() FRaceLap BestLap;

	FORCEINLINE void Update(int8 NewPosition, FRaceLap NewBestLap)
	{
		Position = NewPosition;
		BestLap = NewBestLap;
	}
	
public:

	FTimeAttackLeaderboardRow() = default;
	
	FTimeAttackLeaderboardRow(const int32 InPlayerId, const FString InPlayerName, FRaceLap InBestLap)
		: PlayerId(InPlayerId), PlayerName(InPlayerName), BestLap(InBestLap)
	{}
	
};
