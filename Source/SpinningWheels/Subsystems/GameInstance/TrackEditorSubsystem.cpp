// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackEditorSubsystem.h"

void UTrackEditorSubsystem::SetNextTrackToLoad(const FTrackSaveData& InTrackToLoad)
{
	TrackToLoad = InTrackToLoad;
	bTrackToLoad = true;
}

TOptional<FTrackSaveData> UTrackEditorSubsystem::GetTrackToLoad(bool bClear)
{
	if (bTrackToLoad == true)
	{
		Clear();
		return TrackToLoad;
	}
	return TOptional<FTrackSaveData>();
}

void UTrackEditorSubsystem::Clear()
{
	bTrackToLoad = false;
}
