// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorGameMode.h"

#include "JsonObjectConverter.h"
#include "SpinningWheels/Actors/TrackGrid.h"
#include "SpinningWheels/Controllers/RaceController.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/Subsystems/GameInstance/TrackEditorSubsystem.h"

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
					TrackGrid->Initialize(GridSize.X, GridSize.Y, TrackToLoad.GetValue());
					OnTrackGridReady.Broadcast(TrackGrid.Get());
					return;
				}
			}
		}
		
		// Simulating a map load (it works)
		// const FString BaseDir = FPaths::ProjectUserDir() / TEXT("Tracks"); // User/Documents on Live
		// const FString FileName = TEXT("rotation.json");
		// const FString FilePath = BaseDir / FileName;
		//
		// if (FPaths::FileExists(FilePath))
		// {
		// 	FString OutString;
		// 	if (FFileHelper::LoadFileToString(OutString, *FilePath))
		// 	{
		// 		FTrackSaveData TrackSave;
		// 		if (FJsonObjectConverter::JsonObjectStringToUStruct(OutString, &TrackSave))
		// 		{
		// 			UE_LOG(LogTemp, Warning, TEXT("dunno how, but it works %s %s"), *TrackSave.Id.ToString(), *TrackSave.Name);
		// 			TrackGrid->Initialize(2, 3, TrackSave);
		// 		}
		// 	}
		// }
		// End simulating a map load

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

	EditorMode = EEditorMode::EM_TestTrack;
	
	RestartPlayer(Controller);
}

void AEditorGameMode::SaveTrack(const FString& TrackName)
{
	if (TrackGrid.IsValid())
	{
		if (CurrentTrack.IsNull())
		{
			const FGuid TrackGuid = FGuid::NewGuid();
			CurrentTrack = FTrack(TrackGuid, TrackName);
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
		if (const bool bSuccess = FJsonObjectConverter::UStructToJsonObjectString(TrackSave, OutputJson); bSuccess == false)
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

		const bool bSaved = FFileHelper::SaveStringToFile(OutputJson, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8);
		OnTrackSaved.Broadcast(CurrentTrack, bSaved);
		
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
