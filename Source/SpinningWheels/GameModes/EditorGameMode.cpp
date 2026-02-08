// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorGameMode.h"

#include "JsonObjectConverter.h"
#include "SpinningWheels/Actors/TrackGrid.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/Pawns/EditorPawn.h"
#include "SpinningWheels/Subsystems/GameInstance/TrackEditorSubsystem.h"
#include "SpinningWheels/Subsystems/GameInstance/TracksSubsystem.h"

void AEditorGameMode::SetEditorMode(EEditorMode NewEditorMode)
{
	EditorMode = NewEditorMode;

	OnEditorModeChanged.Broadcast(EditorMode);
}

void AEditorGameMode::TriggerRefreshTracks()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UTracksSubsystem* TracksSubsystem = GameInstance->GetSubsystem<UTracksSubsystem>())
		{
			TracksSubsystem->RefreshTracks();
		}
	}
}

void AEditorGameMode::PrepareControllerForNewLap(AController* Controller)
{
	if (EditorMode == EEditorMode::EM_TestTrack)
	{
		if (ARaceController* RC = Cast<ARaceController>(Controller))
		{
			RC->PrepareForNewLap(GetWorld()->GetTimeSeconds() + 4.f);
		}
	}
}

void AEditorGameMode::StartPlay()
{
	Super::StartPlay();

	InitializeGrid();
}

void AEditorGameMode::InitializeGrid()
{
	TrackGrid = GetWorld()->SpawnActor<ATrackGrid>(DefaultTrackGridClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (TrackGrid.IsValid())
	{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (UTrackEditorSubsystem* TrackEditorSubsystem = GameInstance->GetSubsystem<UTrackEditorSubsystem>())
			{
				TOptional<FTrackSaveData> TrackToLoad = TrackEditorSubsystem->GetTrackToLoad(true);
				if (TrackToLoad.IsSet())
				{
					FTrackSaveData TrackToLoadSaveData = TrackToLoad.GetValue();
					CurrentTrack = FTrack(TrackToLoadSaveData.Id, TrackToLoadSaveData.Name);

					TrackGrid->Initialize(GridSize.X, GridSize.Y, TrackToLoad.GetValue());
					OnTrackGridReady.Broadcast(TrackGrid.Get());

					OnTrackLoaded.Broadcast(CurrentTrack);

					return;
				}
			}
		}

		TrackGrid->Initialize(GridSize.X, GridSize.Y);
		OnTrackGridReady.Broadcast(TrackGrid.Get());
	}
}

void AEditorGameMode::TestTrack(AController* Controller)
{
	DefaultPawnClass = DefaultCarClass;
	if (APawn* ControlledPawn = Controller->GetPawn())
	{
		ControlledPawn->Destroy();
	}

	SetEditorMode(EEditorMode::EM_TestTrack);

	RestartPlayer(Controller);
}

void AEditorGameMode::ReturnToEditor(AController* Controller)
{
	DefaultPawnClass = DefaultEditorClass;
	if (APawn* ControlledPawn = Controller->GetPawn())
	{
		ControlledPawn->Destroy();
	}

	SetEditorMode(EEditorMode::EM_Editor);

	RestartPlayer(Controller);
}

void AEditorGameMode::SaveTrack(const FString& TrackName)
{
	if (TrackGrid.IsValid())
	{
		bool bRefreshTracks = false;

		if (CurrentTrack.IsNull())
		{
			const FGuid TrackGuid = FGuid::NewGuid();
			CurrentTrack = FTrack(TrackGuid, TrackName);

			bRefreshTracks = true;
		}
		else if (CurrentTrack.Name != TrackName)
		{
			// The name of this track has changed
			bRefreshTracks = true;
		}
		
		CurrentTrack.Name = TrackName;

		FTrackSaveData TrackSave;
		TrackSave.Version = 1;
		TrackSave.Id = CurrentTrack.Id;
		TrackSave.Name = CurrentTrack.Name;

		const TArray<FTrackBlock> Blocks = TrackGrid->GetBlocks();
		TArray<FTrackBlockSaveData> BlocksSaveData;
		BlocksSaveData.SetNum(Blocks.Num());
		for (int i = 0; i < Blocks.Num(); i++)
		{
			BlocksSaveData[i] = FTrackBlockSaveData(Blocks[i].BlockId, Blocks[i].Coordinates);
		}

		TrackSave.Blocks = BlocksSaveData;

		FString OutputJson;
		const bool bSuccess = FJsonObjectConverter::UStructToJsonObjectString(TrackSave, OutputJson);
		if (bSuccess == false)
		{
			OnTrackSaved.Broadcast(CurrentTrack, bSuccess);
			return;
		}

		const FString BaseDir = FPaths::ProjectUserDir() / TEXT("Tracks"); // User/Documents on Live
		const FString FileName = FPaths::MakeValidFileName(CurrentTrack.Id.ToString()) + TEXT(".json");
		const FString FilePath = BaseDir / FileName;

		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (PlatformFile.DirectoryExists(*BaseDir) == false)
		{
			PlatformFile.CreateDirectory(*BaseDir);
		}

		const bool bSaved = FFileHelper::SaveStringToFile(OutputJson, *FilePath,
		                                                  FFileHelper::EEncodingOptions::ForceUTF8);
		OnTrackSaved.Broadcast(CurrentTrack, bSaved);

		// Trigger a refresh of all tracks available
		if (bRefreshTracks == true)
		{
			TriggerRefreshTracks();
		}
	}
}

FString AEditorGameMode::GetTrackName()
{
	if (CurrentTrack.IsNull())
	{
		return "";
	}

	return CurrentTrack.Name;
}
