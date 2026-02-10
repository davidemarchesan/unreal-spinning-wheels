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

public:

	UPROPERTY() bool bInitialized = false;
	UPROPERTY() int32 BestLapTime = 0;
	UPROPERTY() TArray<int32> BestSectors;
	UPROPERTY() TArray<FRaceLap> PlayersBestLap;

private:

	void CalcBestSectors();

public:

	void Reset();

	void AddPlayerNewBest(FRaceLap NewLap);
	
	const TArray<int32>& GetBestSectors() const { return BestSectors; }
	const TArray<FRaceLap>& GetPlayersBestLap() const { return PlayersBestLap; }

	FString ToString() const;
	
};

FORCEINLINE void FTimeAttackLeaderboard::CalcBestSectors()
{

	if (BestSectors.Num() <= 1)
	{
		return;
	}

	TArray<int32> TempBestSectors;
	// TempBestSectors.SetNum(BestSectors.Num());
	
	for (int32 i = 0; i < BestSectors.Num(); i++)
	{

		int32 Best = 0;

		for (int32 j = 0; j < PlayersBestLap.Num(); j++)
		{
			const TArray<int32> PlayerSectors = PlayersBestLap[j].GetSectors();
			if (PlayerSectors.IsValidIndex(i))
			{
				if (Best == 0)
				{
					Best = PlayerSectors[i];
				}
				else
				{
					Best = FMath::Min(Best, PlayerSectors[i]);
				}
			}
		}

		TempBestSectors.Add(Best);
	}

	BestSectors = TempBestSectors;
	
}


FORCEINLINE void FTimeAttackLeaderboard::Reset()
{
	BestLapTime = 0;
	BestSectors.Reset();
	PlayersBestLap.Reset();
}


FORCEINLINE void FTimeAttackLeaderboard::AddPlayerNewBest(FRaceLap NewLap)
{
	TArray<FRaceLap> TempPlayersBestLap = PlayersBestLap;
	int32 FoundIndex = TempPlayersBestLap.IndexOfByPredicate([NewLap](const FRaceLap& InLap)
	{
		return InLap.GetPlayerId() == NewLap.GetPlayerId();
	});

	if (FoundIndex == INDEX_NONE)
	{
		TempPlayersBestLap.Add(NewLap);
	}
	else
	{
		if (TempPlayersBestLap.IsValidIndex(FoundIndex))
		{
			TempPlayersBestLap[FoundIndex] = NewLap;
		}
	}

	TempPlayersBestLap.Sort([](const FRaceLap& L1, const FRaceLap& L2)
	{
		if (L1 == L2)
		{
			 return L1.GetSetTime() < L2.GetSetTime();
		}

		return L1 < L2;
	});

	PlayersBestLap = TempPlayersBestLap;

	if (BestSectors.Num() == 0)
	{
		BestSectors = NewLap.GetSectors();
	}

	CalcBestSectors();

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
