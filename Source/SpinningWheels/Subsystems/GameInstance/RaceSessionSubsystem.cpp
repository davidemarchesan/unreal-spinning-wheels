// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

void URaceSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GEngine)
	{
		GEngine->NetworkFailureEvent.AddUObject(this, &URaceSessionSubsystem::OnNetworkFailureEvent);
	}

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			OnSessionUserInviteAcceptedDelegateHandle = SessionInterface->OnSessionUserInviteAcceptedDelegates.AddLambda([this](const bool bWasSuccessful, const int32 ControllerId, const FUniqueNetIdPtr& UserId, const FOnlineSessionSearchResult& InviteResult)
			{
				OnSessionUserInviteAccepted(bWasSuccessful, ControllerId, UserId, InviteResult);
			});
		}
	}
}

void URaceSessionSubsystem::Deinitialize()
{
	if (GEngine)
	{
		GEngine->NetworkFailureEvent.RemoveAll(this);
	}

	if (SessionInterface.IsValid())
	{
		SessionInterface->OnSessionUserInviteAcceptedDelegates.Remove(OnSessionUserInviteAcceptedDelegateHandle);
	}

	Super::Deinitialize();
}

void URaceSessionSubsystem::GoToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Levels/L_Main");
}

void URaceSessionSubsystem::ClearViewport()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
	}
}

void URaceSessionSubsystem::OnNetworkFailureEvent(UWorld* World, UNetDriver* NetDrive,
                                                  ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	if (SessionInterface.IsValid() && bSession == true)
	{
		DestroySteamSession();
	}
	else
	{
		ClearViewport();
		GoToMainMenu();
	}
}

void URaceSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface && OnCreateSessionCompleteDelegateHandle.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.Remove(OnCreateSessionCompleteDelegateHandle);
	}

	if (bWasSuccessful == true)
	{
		bHost = true;
		bSession = true;
		ClearViewport();
		GetWorld()->ServerTravel("/Game/Levels/L_Race?listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("URaceSessionSubsystem::OnCreateSessionComplete | Could not create online session!"));

		ClearViewport();
		GoToMainMenu();
	}
}

void URaceSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface && OnDestroySessionCompleteDelegateHandle.IsValid())
	{
		SessionInterface->OnDestroySessionCompleteDelegates.Remove(OnDestroySessionCompleteDelegateHandle);
	}

	if (bWasSuccessful == true)
	{
		ClearViewport();
		GoToMainMenu();

		bHost = false;
		bSession = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("URaceSessionSubsystem::OnDestroySessionComplete | Could not destroy online session!"));
	}
}

void URaceSessionSubsystem::OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
	const FUniqueNetIdPtr& UserId, const FOnlineSessionSearchResult& InviteResult)
{
	if (bWasSuccessful == false)
	{
		return;
	}

	if (SessionInterface.IsValid())
	{
		OnJoinSessionCompleteDelegateHandle = SessionInterface->OnJoinSessionCompleteDelegates.AddLambda([this](FName SessionName, EOnJoinSessionCompleteResult::Type Result)
		{
			OnJoinSessionComplete(SessionName, Result);
		});
		SessionInterface->JoinSession(*UserId, NAME_GameSession, InviteResult);
	}
}

void URaceSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid() && OnJoinSessionCompleteDelegateHandle.IsValid())
	{
		SessionInterface->OnJoinSessionCompleteDelegates.Remove(OnJoinSessionCompleteDelegateHandle);
	}


	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		return;
	}

	FString ConnectionString;

	if (SessionInterface->GetResolvedConnectString(SessionName, ConnectionString))
	{
		bSession = true;
		
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			ClearViewport();
			PlayerController->ClientTravel(ConnectionString, TRAVEL_Absolute);
		}
	}
	
}

void URaceSessionSubsystem::StartLANSession()
{
	ClearViewport();
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Levels/L_Race?listen");
}

void URaceSessionSubsystem::StartSteamSession()
{
	if (SessionInterface.IsValid() == false)
	{
		return;
	}

	// Check if we already are in a session
	if (SessionInterface->GetNamedSession(NAME_GameSession) != nullptr)
	{
		DestroySteamSession();
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 10;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;

	if (UWorld* World = GetWorld())
	{
		if (ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController())
		{
			FUniqueNetIdRepl NetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
			if (NetIdRepl.IsValid() == false)
			{
				return;
			}

			const FUniqueNetId& NetId = *NetIdRepl.GetUniqueNetId();
			if (NetId.IsValid() == false)
			{
				return;
			}

			if (SessionInterface.IsValid())
			{
				
				OnCreateSessionCompleteDelegateHandle = SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
					this, &URaceSessionSubsystem::OnCreateSessionComplete);

				if (SessionInterface->CreateSession(
					NetId,
					NAME_GameSession,
					SessionSettings) == false)
				{
					if (SessionInterface && OnCreateSessionCompleteDelegateHandle.IsValid())
					{
						SessionInterface->OnCreateSessionCompleteDelegates.Remove(OnCreateSessionCompleteDelegateHandle);
					}
				}
			}
		}
	}
}

void URaceSessionSubsystem::DestroySteamSession()
{
	if (SessionInterface.IsValid())
	{
		if (SessionInterface->GetNamedSession(NAME_GameSession) != nullptr)
		{
			OnDestroySessionCompleteDelegateHandle = SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
				this, &URaceSessionSubsystem::OnDestroySessionComplete);

			if (SessionInterface->DestroySession(NAME_GameSession) == false)
			{
				if (SessionInterface && OnDestroySessionCompleteDelegateHandle.IsValid())
				{
					SessionInterface->OnDestroySessionCompleteDelegates.Remove(OnDestroySessionCompleteDelegateHandle);
				}
			}
		}
		else
		{
			bHost = false;
			bSession = false;
			ClearViewport();
			GoToMainMenu();
		}
	}
}

void URaceSessionSubsystem::LeaveSession()
{
	if (SessionInterface.IsValid() && bSession == true)
	{
		DestroySteamSession();
	}
	else
	{
		ClearViewport();
		GoToMainMenu();
	}
}
