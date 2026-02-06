// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingSubsystem.h"

#include "TracksSubsystem.h"

void ULoadingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Dependencies
	Collection.InitializeDependency(UTracksSubsystem::StaticClass());

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		TracksSubsystem = GameInstance->GetSubsystem<UTracksSubsystem>();

		if (TracksSubsystem.IsValid())
		{
			if (TracksSubsystem->IsReady())
			{
				OnTracksSubsystemReady();
			}
			else
			{
				OnTracksSubsystemReadyDelegateHandle = TracksSubsystem->OnReady.AddUObject(
					this, &ULoadingSubsystem::OnTracksSubsystemReady);
			}
		}
	}
}

void ULoadingSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (OnTracksSubsystemReadyDelegateHandle.IsValid())
	{
		TracksSubsystem->OnReady.Remove(OnTracksSubsystemReadyDelegateHandle);
		OnTracksSubsystemReadyDelegateHandle.Reset();
		TracksSubsystem.Reset();
	}
}

void ULoadingSubsystem::CheckIfEverythingIsReady()
{
	if (Ready >= ReadyNeeded)
	{
		ImReady();
	}
}

void ULoadingSubsystem::ImReady()
{
	bIsEverythingReady = true;
	OnLoadingSubsystemReady.Broadcast();
}

void ULoadingSubsystem::OnTracksSubsystemReady()
{
	Ready++;
	CheckIfEverythingIsReady();
}
