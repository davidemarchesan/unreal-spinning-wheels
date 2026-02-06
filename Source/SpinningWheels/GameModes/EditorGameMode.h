// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceGameModeBase.h"
#include "SpinningWheels/Core/Track.h"
#include "EditorGameMode.generated.h"

UENUM()
enum class EEditorMode : uint8
{
	EM_Editor,
	EM_TestTrack
};

class ACar;
class AEditorPawn;
class ATrackGrid;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackGridReadySignature, ATrackGrid*, TrackGrid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrackSavedSignature, const FTrack&, CurrentTrack, const bool, bSuccess);

UCLASS()
class SPINNINGWHEELS_API AEditorGameMode : public ARaceGameModeBase
{
	GENERATED_BODY()

private:

	EEditorMode EditorMode = EEditorMode::EM_Editor;

	TWeakObjectPtr<ATrackGrid> TrackGrid;

	FTrack CurrentTrack;
	
protected:

	virtual void PrepareControllerForNewLap(AController* Controller) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<ACar> DefaultCarClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<AEditorPawn> DefaultEditorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<ATrackGrid> DefaultTrackGridClass;

	UPROPERTY(EditDefaultsOnly, Category="Track")
	FVector2D GridSize = FVector2D(10.f, 10.f);

public:

	virtual void StartPlay() override;

	void InitializeGrid();

	FOnTrackGridReadySignature OnTrackGridReady;
	FOnTrackSavedSignature OnTrackSaved;
	
	TWeakObjectPtr<ATrackGrid> GetTrackGrid() const { return TrackGrid; }

	void TestTrack(AController* Controller);
	void SaveTrack(const FString& TrackName);

	FString GetTrackName();
	
};
