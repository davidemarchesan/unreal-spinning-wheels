// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpinningWheels/Core/Track.h"
#include "RaceGameModeBase.generated.h"

class ATrackGrid;


UCLASS()
class SPINNINGWHEELS_API ARaceGameModeBase : public AGameMode
{
	GENERATED_BODY()

private:

protected:

	FTrack CurrentTrack;

	TWeakObjectPtr<ATrackGrid> TrackGrid;
	TWeakObjectPtr<class AStartBlock> StartBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<ATrackGrid> DefaultTrackGridClass;

	virtual void PrepareControllerForNewLap(AController* Controller);

public:

	//~ Begin AGameModeBase Interface
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	//~ End AGameModeBase Interface

	virtual void CancelLap(AController* Controller);
	
};
