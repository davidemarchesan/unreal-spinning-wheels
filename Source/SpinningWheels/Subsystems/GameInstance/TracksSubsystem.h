// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpinningWheels/Core/Track.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TracksSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTracksSubsystemReadySignature);

UCLASS()
class SPINNINGWHEELS_API UTracksSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	void ImReady();

	bool bReady = false;
	
	TArray<FTrackSaveData> Tracks;

protected:

public:

	bool IsReady() const { return bReady; }
	FOnTracksSubsystemReadySignature OnReady;
	
	TArray<FTrackSaveData> GetTracks() const { return Tracks; }

	FOnTracksSubsystemReadySignature OnTracksSubsystemReady;
	
};
