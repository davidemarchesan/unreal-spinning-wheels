// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLoadingSubsystemReadySignature);

UCLASS()
class SPINNINGWHEELS_API ULoadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	int32 Ready = 0;
	int32 ReadyNeeded = 1;

	bool bIsEverythingReady = false;
	void CheckIfEverythingIsReady();
	void ImReady();
	
	TWeakObjectPtr<class UTracksSubsystem> TracksSubsystem;

	void OnTracksSubsystemReady();
	FDelegateHandle OnTracksSubsystemReadyDelegateHandle;

protected:

public:

	bool IsReady() const { return bIsEverythingReady; }

	FOnLoadingSubsystemReadySignature OnLoadingSubsystemReady;
	
};
