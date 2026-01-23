#pragma once

struct FRaceLap
{
private:
	float StartTime = 0.f;
	float EndTime = 0.f;
	float LapTime = 0.f;
	TArray<float> Sectors;

	bool bClosed = false;

public:
	
	/**
	 * Add a sector to the lap.
	 * @param Time Time of the sector.
	 */
	FORCEINLINE void Sector(const float Time)
	{
		if (bClosed == true)
		{
			return;
		}

		Sectors.Add(Time);
	}

	/**
	 * Close a lap. Calc the total time.
	 * Cannot be edited after being closed.
	 */
	FORCEINLINE void Close(const float Time)
	{
		if (bClosed == true)
		{
			return;
		}

		bClosed = true;

		EndTime = Time;
		LapTime = EndTime - StartTime;
	}

	/**
	 * Compare two lap times
	*/
	FORCEINLINE bool operator>=(const FRaceLap& RL) const
	{
		return LapTime >= RL.LapTime;
	}
	FORCEINLINE bool operator<(const FRaceLap& RL) const
	{
		return LapTime < RL.LapTime;
	}

	FRaceLap() = default;
	
	explicit FRaceLap(const float InStartTime)
		: StartTime(InStartTime)
	{}

public:
	FORCEINLINE float GetStartTime() const { return StartTime;}
	FORCEINLINE float GetEndTime() const { return EndTime;}
	FORCEINLINE float GetLapTime() const { return LapTime; }
	FORCEINLINE TArray<float> GetSectors() const { return Sectors; }

	FString ToString() const;
};

FORCEINLINE FString FRaceLap::ToString() const
{
	FString Str = FString::Printf(TEXT("S:%f E:%f L:%f"), StartTime, EndTime, LapTime);

	for (int i = 0; i < Sectors.Num(); i++)
	{
		Str += FString::Printf(TEXT("\nS%d: %f"), i, Sectors[i]);	
	}
	
	return Str;
}

