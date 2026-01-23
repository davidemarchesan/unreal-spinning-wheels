// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"
#include "SpinningWheels/Actors/Blocks/StartBlock.h"
#include "SpinningWheels/Core/Match.h"

void ARaceGameMode::StartWaitingForPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("ARaceGameMode::StartWaitingForPlayers"));
	GetWorld()->GetTimerManager().SetTimer(WaitingForPlayersTimer, this, &ARaceGameMode::StopWaitingForPlayers, 5.f, false);
}

void ARaceGameMode::StopWaitingForPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("ARaceGameMode::StopWaitingForPlayers"));
	GetWorld()->GetTimerManager().ClearTimer(WaitingForPlayersTimer);
}

void ARaceGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	UE_LOG(LogTemp, Warning, TEXT("ARaceGameMode::HandleMatchHasStarted"));
	StartWaitingForPlayers();
}

void ARaceGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	UWorld* World = GetWorld();
	AStartBlock* StartBlock = nullptr;

	for (TActorIterator<AStartBlock> It(World); It; ++It)
	{
		AStartBlock* Start = *It;
		if (Start)
		{
			StartBlock = Start;
		}
	}

	FVector OutLocation;
	FRotator OutRotation;
	FTransform SpawnTransform;

	if (StartBlock)
	{
		StartBlock->GetStartingPoint(OutLocation, OutRotation);
		
		SpawnTransform.SetLocation(OutLocation);
		SpawnTransform.SetRotation(OutRotation.Quaternion());
	} 

	if (NewPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("RestartPlayer controller is null"));
	}
	RestartPlayerAtTransform(NewPlayer, SpawnTransform);
}

void ARaceGameMode::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtTransform %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtTransform: Tried to restart a spectator-only player!"));
		return;
	}

	FRotator SpawnRotation = SpawnTransform.GetRotation().Rotator();

	if (NewPlayer->GetPawn() != nullptr)
	{
		// If we have an existing pawn, just use it's rotation
		// SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
		// UE_LOG(LogTemp, Warning, TEXT("RestartPlayerAtTransform %s"), *SpawnTransform.GetLocation().ToString());
		NewPlayer->GetPawn()->SetActorLocation(SpawnTransform.GetLocation());
	}
	else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);
		}
	}

	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		FinishRestartPlayer(NewPlayer, SpawnRotation);
	}
}

void ARaceGameMode::CancelLap(AController* Controller)
{
	RestartPlayer(Controller);
}
