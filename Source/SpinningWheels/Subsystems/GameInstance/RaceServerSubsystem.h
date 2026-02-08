// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpinningWheels/Core/Track.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RaceServerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API URaceServerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

	TArray<FTrackSaveData> Tracks;
	int32 CurrentTrackIndex = INDEX_NONE;

protected:

public:

	void SetTracksPlaylist(const TArray<FTrackSaveData>& InTracks);

	FTrackSaveData GetCurrentTrack();
	int32 GetCurrentTrackIndex() const { return CurrentTrackIndex; }
	int32 GoToNextTrack();
	
};
