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

USTRUCT()
struct FTimeAttackLeaderboard
{
	GENERATED_BODY()

private:

	UPROPERTY() int32 BestLapTime = 0;
	UPROPERTY() TArray<int32> BestSectors;
	UPROPERTY() TArray<FRaceLap> PlayersBestLap;

public:

	void AddPlayerNewBest(FRaceLap NewLap);

	FString ToString() const;
	
};

FORCEINLINE void FTimeAttackLeaderboard::AddPlayerNewBest(FRaceLap NewLap)
{

	int32 FoundIndex = PlayersBestLap.IndexOfByPredicate([NewLap](const FRaceLap& InLap)
	{
		return InLap.GetPlayerId() == NewLap.GetPlayerId();
	});

	if (FoundIndex == INDEX_NONE)
	{
		PlayersBestLap.Add(NewLap);
	}
	else
	{
		if (PlayersBestLap.IsValidIndex(FoundIndex))
		{
			PlayersBestLap[FoundIndex] = NewLap;
		}
	}

	PlayersBestLap.Sort([](const FRaceLap& L1, const FRaceLap& L2)
	{
		if (L1 == L2)
		{
			 return L1.GetSetTime() < L2.GetSetTime();
		}

		return L1 < L2;
	});
	
}

FORCEINLINE FString FTimeAttackLeaderboard::ToString() const
{
	FString Str;

	for (int i = 0; i < PlayersBestLap.Num(); i++)
	{
		Str += FString::Printf(TEXT("\nP%d PID%d T%d"), i + 1, PlayersBestLap[i].GetPlayerId(), PlayersBestLap[i].GetLapTime());	
	}
	
	return Str;
}
