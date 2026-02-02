// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpinningWheels/Core/Track.h"
#include "EditorGameMode.generated.h"

class ATrackGrid;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackGridReadySignature, ATrackGrid*, TrackGrid);

UCLASS()
class SPINNINGWHEELS_API AEditorGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	TWeakObjectPtr<ATrackGrid> TrackGrid;

	FTrack CurrentTrack;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<ATrackGrid> DefaultTrackGridClass;

public:

	virtual void StartPlay() override;

	FOnTrackGridReadySignature OnTrackGridReady;
	TWeakObjectPtr<ATrackGrid> GetTrackGrid() const { return TrackGrid; }

	void SaveTrack(const FString& TrackName);
	
};
