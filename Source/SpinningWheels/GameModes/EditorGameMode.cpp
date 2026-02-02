// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorGameMode.h"

#include "JsonObjectConverter.h"
#include "SpinningWheels/Actors/TrackGrid.h"

void AEditorGameMode::StartPlay()
{
	Super::StartPlay();

	TrackGrid = GetWorld()->SpawnActor<ATrackGrid>(DefaultTrackGridClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (TrackGrid.IsValid())
	{

		// Simulating a map load (it works)
		// const FString BaseDir = FPaths::ProjectUserDir() / TEXT("Tracks"); // User/Documents on Live
		// const FString FileName = TEXT("loadtest.json");
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

		TrackGrid->Initialize(2, 3);
		OnTrackGridReady.Broadcast(TrackGrid.Get());
	}
}

void AEditorGameMode::SaveTrack(const FString& TrackName)
{
	if (TrackGrid.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("gamemode save track %s"), *TrackName);
		// access track grid
		// get tiles status
		// currentrack.id ? create n save : overwrite
		// contruct save file (
		// save

		// todo: check if filename string is empty
		if (CurrentTrack.IsNull())
		{
			const FGuid TrackGuid = FGuid::NewGuid();
			UE_LOG(LogTemp, Warning, TEXT("current track is null, creating a new id %s"), *TrackGuid.ToString());
			CurrentTrack = FTrack(TrackGuid, TrackName);
		}

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
		bool bSuccess = FJsonObjectConverter::UStructToJsonObjectString(TrackSave, OutputJson);

		UE_LOG(LogTemp, Warning, TEXT("gamemode track to json %d"), bSuccess);
		
		const FString BaseDir = FPaths::ProjectUserDir() / TEXT("Tracks"); // User/Documents on Live
		const FString FileName = FPaths::MakeValidFileName(CurrentTrack.Name) + TEXT(".json");
		const FString FilePath = BaseDir / FileName;

		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (PlatformFile.DirectoryExists(*BaseDir) == false)
		{
			PlatformFile.CreateDirectory(*BaseDir);
		}
		
		bool bSaved = FFileHelper::SaveStringToFile(OutputJson, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8);
		UE_LOG(LogTemp, Warning, TEXT("gamemode save track on %s success %d"), *FilePath, bSaved);
	}
	
}
