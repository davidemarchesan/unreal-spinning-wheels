// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceServerSubsystem.h"

void URaceServerSubsystem::SetTracksPlaylist(const TArray<FTrackSaveData>& InTracks)
{
	Tracks = InTracks;
	CurrentTrackIndex = 0; // Starting track
}

FTrackSaveData URaceServerSubsystem::GetCurrentTrack()
{
	if (Tracks.IsValidIndex(CurrentTrackIndex))
	{
		return Tracks[CurrentTrackIndex];
	}

	return FTrackSaveData();
}

int32 URaceServerSubsystem::GoToNextTrack()
{
	CurrentTrackIndex++;
	return CurrentTrackIndex;
}
