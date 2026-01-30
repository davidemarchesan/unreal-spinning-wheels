// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorGameMode.h"

#include "SpinningWheels/Actors/TrackGrid.h"

void AEditorGameMode::StartPlay()
{
	Super::StartPlay();

	TrackGrid = GetWorld()->SpawnActor<ATrackGrid>(DefaultTrackGridClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (TrackGrid.IsValid())
	{
		TrackGrid->Initialize(10, 10);
	}
}
