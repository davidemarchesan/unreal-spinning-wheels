// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Online/CoreOnline.h"
#include "RaceSessionSubsystem.generated.h"

class FOnlineSessionSearchResult;
class FUniqueNetId;

UCLASS()
class SPINNINGWHEELS_API URaceSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:

	IOnlineSessionPtr SessionInterface;

	bool bHost = false;
	bool bSession = false;

	void GoToMainMenu();
	void ClearViewport();

	UFUNCTION()
	void OnNetworkFailureEvent(UWorld* World, UNetDriver* NetDrive, ENetworkFailure::Type FailureType, const FString& ErrorString);

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	// UFUNCTION()
	void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, const FUniqueNetIdPtr& UserId, const FOnlineSessionSearchResult& InviteResult);
	// void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);

	FDelegateHandle OnSessionUserInviteAcceptedDelegateHandle;

	// UFUNCTION()
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	
protected:

public:

	void StartLANSession();
	
	void StartSteamSession();
	void DestroySteamSession();

	void LeaveSession();
	
};
