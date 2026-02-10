// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "SpinningWheels/Core/Leaderboard.h"
#include "SpinningWheels/Core/Match.h"
#include "RaceController.generated.h"

class UInputMappingContext;
class UDriveInputConfig;
class UGeneralInputConfig;
class ACar;
class AMainCamera;

class ARaceGameMode;
class ARaceGameState;
class ARacePlayerState;
class ARaceHUD;

UENUM()
enum class ERaceControllerPhase : uint8
{
	RCP_Respawning,
	RCP_InStartingProcedure,
	RCP_Driving,
};

UCLASS()
class SPINNINGWHEELS_API ARaceController : public APlayerController
{
	GENERATED_BODY()

public:
	ARaceController();

	virtual void Tick(float DeltaSeconds) override;

private:

	// Begin Ready status check
	UPROPERTY(ReplicatedUsing=OnRep_bReady)
	bool bReady = false;

	bool bLocalReady = false;

	UFUNCTION()
	void OnRep_bReady();
	
	void CheckIfReady();

	UFUNCTION(Server, Reliable)
	void ServerImReady();

	void InternalImReady();
	// End Ready status check

	// Begin Sync leaderboard on-late join
	void SyncLeaderboard();
	
	UFUNCTION(Server, Reliable)
	void ServerSyncLeaderboard();

	UFUNCTION(Client, Reliable)
	void ClientSyncLeaderboard(const FTimeAttackLeaderboard& Leaderboard);
	// End Sync leaderboard on-late joi
	
	// Begin Deterministic physics
	void SimulatedTick(float DeltaSeconds);
	float AccSimulationTime = 0.f;
	float LastSimulationDelta = 0.f;
	float TotSeconds = 0.f;
	// End Deterministic physics

	UPROPERTY(Category=Classes, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ACar> CarClass; // Deprecated

	UPROPERTY(Category=Classes, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AMainCamera> CameraClass;

	UPROPERTY(ReplicatedUsing=OnRep_Phase, VisibleAnywhere)
	ERaceControllerPhase Phase = ERaceControllerPhase::RCP_Respawning;

	UFUNCTION()
	void OnRep_Phase();

	UPROPERTY(Replicated)
	float ServerStartDriveTime = 0.f;

	UPROPERTY(ReplicatedUsing=OnRep_ServerRacingEndTime)
	float ServerRacingEndTime = 0.f;

	void SyncServerRacingEndTime(const bool bForceRefresh = false);

	UFUNCTION()
	void OnRep_ServerRacingEndTime();

	void StartDriveProcedure(float DeltaSeconds);
	int32 StartDriveSecondsRemaining = 0;

	void StartLap();
	void LocalStartLap();
	
	UFUNCTION(Server, Reliable)
	void ServerStartLap();

	/** Input Actions handler - Drive */
	void InputStartDrive();
	void InputStopDrive();
	void InputStartBrake();
	void InputStopBrake();
	void InputTurn(const FInputActionValue& Value);
	void InputStartTurbo();
	void InputStopTurbo();
	void InputCancelLap();

	void InputShowLeaderboard();
	void InputHideLeaderboard();

	int8 DriveInputValue = 0;
	int8 BrakeInputValue = 0;
	int8 TurnInputValue = 0;
	int8 TurboInputValue = 0;

	void InternalCancelLap();
	
	UFUNCTION(Server, Reliable)
	void ServerCancelLap();

	/** Begin delegates bindings */
	UFUNCTION() void OnRaceMatchStateUpdate(ERaceMatchState NewState);
	/** End delegates bindings */

	/** Debug */
	void Debug(); // todo: remove

protected:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> DriveMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDriveInputConfig> DriveInputConfig;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> GeneralMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UGeneralInputConfig> GeneralInputConfig;

	//~ Begin AController Interface
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlayingState() override;
	//~ End AController Interface

	virtual void SetDefaultInputMode();

	virtual void SetupInputBindings();
	virtual void SetupDriveInputBindings();
	virtual void SetupGeneralInputBindings();

	virtual void EnableDefaultInputMappingContext();
	virtual void EnableDriveInputMappingContext();
	virtual void DisableDriveInputMappingContext();
	virtual void EnableGeneralInputMappingContext();
	virtual void DisableGeneralInputMappingContext();

	void CreateCamera();

	bool CanCreateCamera() const
	{
		return IsLocalController() == true && bCameraInitialized == false && bCameraInitializing == false;
	}

	bool bCameraInitializing = false;
	bool bCameraInitialized = false;

	TWeakObjectPtr<ACar> Car;
	TWeakObjectPtr<AMainCamera> MainCamera;

	TWeakObjectPtr<ARaceGameState> RaceGameState;
	TWeakObjectPtr<ARacePlayerState> RacePlayerState;
	TWeakObjectPtr<ARaceHUD> RaceHUD;

	ARaceGameMode* GetRaceGameMode();

	void TryGetRaceGameState();
	void OnRaceGameStateInit();
	
	void TryGetRacePlayerState();
	void OnRacePlayerStateInit();
	
	void TryGetRaceHUD();
	void OnRaceHUDInit();

	/** Input Actions handler - Drive */
	virtual void InputOpenMenu();

public:
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Begin AController Interface
	virtual void OnRep_PlayerState() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;
	//~ End AController Interface

	void SetReady(bool bInReady) { bReady = bInReady; }

	void SetPhase(ERaceControllerPhase NewPhase);
	void InternalSetPhase(ERaceControllerPhase NewPhase);

	UFUNCTION(Server, Reliable)
	void ServerSetPhase(ERaceControllerPhase NewPhase);

	void PrepareForNewLap(const float InServerStartTime);
	void SetRacingEndTime(const float InServerRacingEndTime);

};
