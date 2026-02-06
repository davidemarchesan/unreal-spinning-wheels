// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpinningWheels/Core/Track.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TrackEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API UTrackEditorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

	bool bTrackToLoad = false;
	FTrackSaveData TrackToLoad;
	
protected:

public:
	
	void SetNextTrackToLoad(const FTrackSaveData& InTrackToLoad);
	TOptional<FTrackSaveData> GetTrackToLoad(bool bClear = false);
	void Clear();
	
};
