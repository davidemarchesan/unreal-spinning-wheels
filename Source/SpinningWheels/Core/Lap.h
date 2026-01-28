#pragma once

#include "SpinningWheels/Core/Simulation.h"
#include "Lap.generated.h"

USTRUCT()
struct FRaceLap
{
	GENERATED_BODY()
	
private:

	UPROPERTY() int32 PlayerId;
	UPROPERTY() FString PlayerName;

	UPROPERTY() float LapSetAt;
	
	UPROPERTY() int32 LapTime = 0.f;
	UPROPERTY() TArray<int32> Sectors;

	UPROPERTY() bool bClosed = false;

public:
	
	/**
	 * Add a sector to the lap.
	 * @param FrameIndex
	 */
	FORCEINLINE void AddSector(const int32 FrameIndex)
	{
		if (bClosed == true)
		{
			return;
		}

		const int32 Frames = FrameIndex + 1;											// Example: Frame index 100 is the 101st
		const float TimeSeconds = Frames * SimulationConstants::TickFrequency;			// Example: 101 * (1/120) = 0.8416667 seconds
		const int32 TimeMilliSeconds = FMath::FloorToInt32(TimeSeconds * 1000.f);	// Example: 0.8416667 seconds = 841.6667 milliseconds = 842 rounded ms

		if (Sectors.Num() == 0)
		{
			// This is the first sector to be added to the lap.
			// So first sector time is the time until now.
			Sectors.Add(TimeMilliSeconds);
		}
		else
		{
			int32 SectorTime = TimeMilliSeconds;
			for (int i = 0; i < Sectors.Num(); i++)
			{
				SectorTime -= Sectors[i];
			}

			Sectors.Add(SectorTime);
		}
		
	}

	/**
	 * Close a lap. Calc the total time.
	 * Cannot be edited after being closed.
	 */
	FORCEINLINE void Close(const int32 FrameIndex)
	{
		if (bClosed == true)
		{
			return;
		}

		AddSector(FrameIndex);

		for (int32 i = 0; i < Sectors.Num(); i++)
		{
			LapTime += Sectors[i];
		}
		
		bClosed = true;
	}

	/**
	 * Compare two lap times
	*/
	FORCEINLINE bool operator==(const FRaceLap& RL) const
	{
		return LapTime == RL.LapTime;
	}
	FORCEINLINE bool operator>=(const FRaceLap& RL) const
	{
		return LapTime >= RL.LapTime;
	}
	FORCEINLINE bool operator<(const FRaceLap& RL) const
	{
		if (RL.LapTime == 0.f || RL.bClosed == false) return true;
		return LapTime < RL.LapTime;
	}
	FORCEINLINE bool operator<(const int32 OtherLapTime) const
	{
		return LapTime < OtherLapTime;
	}

	FRaceLap() = default;

public:
	FORCEINLINE int32 GetPlayerId() const { return PlayerId; }
	FORCEINLINE int32 GetSetTime() const { return LapSetAt; }
	FORCEINLINE int32 GetLapTime() const { return LapTime; }
	FORCEINLINE TArray<int32> GetSectors() const { return Sectors; }

	FORCEINLINE void SetPlayer(int32 InPlayerId, FString InPlayerName) { PlayerId = InPlayerId; PlayerName = InPlayerName; };
	FORCEINLINE void SetAt(const float ServerTime) { LapSetAt = ServerTime; }

	FString ToString() const;
};

FORCEINLINE FString FRaceLap::ToString() const
{
	FString Str = FString::Printf(TEXT("LapTime:%d"), LapTime);

	for (int i = 0; i < Sectors.Num(); i++)
	{
		Str += FString::Printf(TEXT("\nS%d: %d"), i, Sectors[i]);	
	}
	
	return Str;
}

