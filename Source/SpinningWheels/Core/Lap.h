#pragma once

#include "SpinningWheels/Core/Simulation.h"
#include "Lap.generated.h"

USTRUCT(BlueprintType)
struct FRaceLap
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 PlayerId;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	float LapSetAt;

	UPROPERTY()
	int32 LapTime = 0.f;

	UPROPERTY()
	bool bClosed = false;

	UPROPERTY()
	TArray<int32> Sectors;

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{

		Ar << PlayerId;
		Ar << PlayerName;
		Ar << LapSetAt;
		Ar << LapTime;
		Ar << bClosed;

		int32 NumSectors = Sectors.Num();
		Ar << NumSectors; // Length

		if (Ar.IsLoading())
		{
			Sectors.SetNum(NumSectors);
		}

		for (int32& Sector : Sectors)
		{
			Ar << Sector;
		}
		
		bOutSuccess = true;
		return true;
	}

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

		const int32 Frames = FrameIndex + 1; // Example: Frame index 100 is the 101st
		const float TimeSeconds = Frames * SimulationConstants::TickFrequency;
		// Example: 101 * (1/120) = 0.8416667 seconds
		const int32 TimeMilliSeconds = FMath::FloorToInt32(TimeSeconds * 1000.f);
		// Example: 0.8416667 seconds = 841.6667 milliseconds = 842 rounded ms

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
	FORCEINLINE FString GetPlayerName() const { return PlayerName; }
	FORCEINLINE int32 GetSetTime() const { return LapSetAt; }
	FORCEINLINE int32 GetLapTime() const { return LapTime; }
	FORCEINLINE TArray<int32> GetSectors() const { return Sectors; }

	FORCEINLINE void SetPlayer(int32 InPlayerId, FString InPlayerName)
	{
		PlayerId = InPlayerId;
		PlayerName = InPlayerName;
	};
	FORCEINLINE void SetAt(const float ServerTime) { LapSetAt = ServerTime; }

	static FString FormatTime(int32 Time);
	FORCEINLINE FString GetLapTimeFormat() const { return FormatTime(GetLapTime()); }
	FORCEINLINE FString GetSectorFormat(int32 Index) const
	{
		if (Sectors.IsValidIndex(Index))
		{
			return FormatTime(Sectors[Index]);
		}

		return FString();
	}

	FORCEINLINE static int32 GetDiff(int32 A, int32 B) { return A - B; }
	FORCEINLINE int32 GetLapTimeDiff(int32 Other) const
	{
		if (Other == 0) return 0;
		return GetDiff(LapTime, Other);
	}
	FORCEINLINE int32 GetSectorDiff(int32 Index, int32 Other) const { return GetDiff(Sectors[Index], Other); }
	FORCEINLINE FString static FormatDiff(int32 Diff)
	{
		const FString Sign = Diff < 0 ? "-" : "+";
		const FString Number = FormatTime(FMath::Abs<int32>(Diff));
		FString Str = Sign + " " + Number;

		return Str;
	}
	

	FString ToString() const;
};

FORCEINLINE FString FRaceLap::FormatTime(int32 Time)
{
	FString Str;

	const float Minute = 60.f * 1000.f;

	const int32 Minutes = FMath::FloorToInt(Time / Minute);
	const int32 SecondsMS = Time - (Minute * Minutes);
	const int32 Seconds = FMath::FloorToInt(SecondsMS / 1000.f);
	const int32 MilliSeconds = SecondsMS - (Seconds * 1000.f);

	if (Minutes > 0)
	{
		return FString::Printf(TEXT("%02d:%02d.%03d"), Minutes, Seconds, MilliSeconds);
	}
	else
	{
		return FString::Printf(TEXT("%02d.%03d"), Seconds, MilliSeconds);
	}
}


FORCEINLINE FString FRaceLap::ToString() const
{
	FString Str = FString::Printf(TEXT("LapTime:%d"), LapTime);

	for (int i = 0; i < Sectors.Num(); i++)
	{
		Str += FString::Printf(TEXT("\nS%d: %d"), i, Sectors[i]);
	}

	return Str;
}

template<>
struct TStructOpsTypeTraits<FRaceLap> : public TStructOpsTypeTraitsBase2<FRaceLap>
{
	enum { WithNetSerializer = true };
};

enum class ESlateTimeColor : int {
	TC_White,
	TC_Purple,
	TC_Green,
	TC_Red,
};

struct FSlateTime
{
public:
	FText Time = FText::GetEmpty();
	ESlateTimeColor TimeColor = ESlateTimeColor::TC_White;

	FText Diff = FText::GetEmpty();
	ESlateTimeColor DiffColor = ESlateTimeColor::TC_White;

public:
	FSlateTime() = default;

	FSlateTime(const FText& InTime)
		: Time(InTime)
	{
	}

	FSlateTime(const FText& InTime, const ESlateTimeColor InTimeColor)
		: Time(InTime), TimeColor(InTimeColor)
	{
	}

	FSlateTime(const FText& InTime, const ESlateTimeColor InTimeColor, const FText& InDiff,
	           const ESlateTimeColor InDiffColor)
		: Time(InTime), TimeColor(InTimeColor), Diff(InDiff), DiffColor(InDiffColor)
	{
	}
};

struct FSlateLapTimeRow
{
public:
	FText Name;
	TArray<FSlateTime> Sectors;
	FSlateTime LapTime;

public:

	FSlateLapTimeRow() = default;

	FSlateLapTimeRow(const FText& InName)
		: Name(InName){}
};
