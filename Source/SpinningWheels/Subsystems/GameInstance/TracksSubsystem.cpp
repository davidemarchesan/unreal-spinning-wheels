// Fill out your copyright notice in the Description page of Project Settings.


#include "TracksSubsystem.h"

#include "JsonObjectConverter.h"
#include "SpinningWheels/Core/Track.h"

void UTracksSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const FString BaseDir = FPaths::ProjectUserDir() / TEXT("Tracks"); // User/Documents on Live
	const FString FileExt = "json";

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	TArray<FString> FoundFiles;
	PlatformFile.FindFiles(FoundFiles, *BaseDir, *FileExt);

	for (int32 i = 0; i < FoundFiles.Num(); i++)
	{
		if (FPaths::FileExists(*FoundFiles[i]))
		{
			FString OutString;
			if (FFileHelper::LoadFileToString(OutString, *FoundFiles[i]))
			{
				FTrackSaveData TrackSave;
				if (FJsonObjectConverter::JsonObjectStringToUStruct(OutString, &TrackSave))
				{
					Tracks.Push(TrackSave);
				}
			}
		}
	}

	ImReady();
		
}

void UTracksSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTracksSubsystem::ImReady()
{
	bReady = true;
	OnReady.Broadcast();
}
