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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackLoadedSignature, const FTrack&, CurrentTrack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrackSavedSignature, const FTrack&, CurrentTrack, const bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditorModeChangedSignature, const EEditorMode, EditorMode);

UCLASS()
class SPINNINGWHEELS_API AEditorGameMode : public ARaceGameModeBase
{
	GENERATED_BODY()

private:

	EEditorMode EditorMode = EEditorMode::EM_Editor;

	void SetEditorMode(EEditorMode NewEditorMode);

	void TriggerRefreshTracks();
	
protected:

	virtual void PrepareControllerForNewLap(AController* Controller) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<ACar> DefaultCarClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<AEditorPawn> DefaultEditorClass;

public:

	virtual void StartPlay() override;

	void InitializeGrid();

	FOnTrackGridReadySignature OnTrackGridReady;
	FOnTrackLoadedSignature OnTrackLoaded;
	FOnTrackSavedSignature OnTrackSaved;
	FOnEditorModeChangedSignature OnEditorModeChanged;
	
	TWeakObjectPtr<ATrackGrid> GetTrackGrid() const { return TrackGrid; }

	void TestTrack(AController* Controller);
	void ReturnToEditor(AController* Controller);
	void SaveTrack(const FString& TrackName);

	FTrack GetCurrentTrack() { return CurrentTrack; }
	FString GetTrackName();
	
};
