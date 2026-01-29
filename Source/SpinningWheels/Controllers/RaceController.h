// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "RaceController.generated.h"

class UInputMappingContext;
class UDriveInputConfig;
class ACar;
class AMainCamera;

class ARaceGameMode;
class ARaceGameState;
class ARacePlayerState;

UENUM()
enum class ERaceControllerPhase : uint8
{
	RCP_Respawning,
	RCP_InStartingProcedure,
	RCP_Driving,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartLapCountdownSignature, float, Seconds); // Deprecated
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateLapCountdownSignature, int32, Seconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateRacePlayerStateSignature, ARacePlayerState*, RacePlayerState);

UCLASS()
class SPINNINGWHEELS_API ARaceController : public APlayerController
{
	GENERATED_BODY()

public:
	ARaceController();

	virtual void Tick(float DeltaSeconds) override;

private:
	// Begin Deterministic physics
	void SimulatedTick(float DeltaSeconds);
	float AccSimulationTime = 0.f;
	float LastSimulationDelta = 0.f;
	float TotSeconds = 0.f;
	// End Deterministic physics

	void SetupDriveInputBindings();

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> DriveMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDriveInputConfig> DriveInputConfig;

	UPROPERTY(Category=Classes, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ACar> CarClass;

	UPROPERTY(Category=Classes, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AMainCamera> CameraClass;

	TWeakObjectPtr<ACar> Car;
	TWeakObjectPtr<AMainCamera> MainCamera;

	void CreateCamera();

	bool CanCreateCamera() const
	{
		return IsLocalController() == true && bCameraInitialized == false && bCameraInitializing == false;
	}

	bool bCameraInitializing = false;
	bool bCameraInitialized = false;

	UPROPERTY(ReplicatedUsing=OnRep_Phase, VisibleAnywhere)
	ERaceControllerPhase Phase = ERaceControllerPhase::RCP_Respawning;

	UFUNCTION()
	void OnRep_Phase();

	UPROPERTY(Replicated)
	float ServerStartDriveTime = 0.f;

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

	/** Debug */
	void Debug();

protected:

	//~ Begin AController Interface
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;
	//~ End AController Interface

	TWeakObjectPtr<ARacePlayerState> RacePlayerState;

	ARaceGameMode* GetRaceGameMode();
	ARaceGameState* GetRaceGameState();
	ARacePlayerState* GetRacePlayerState();

public:
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Begin AController Interface
	virtual void OnRep_PlayerState() override;
	virtual void OnPossess(APawn* InPawn) override;
	//~ End AController Interface

	void SetPhase(ERaceControllerPhase NewPhase);

	void InternalSetPhase(ERaceControllerPhase NewPhase);

	UFUNCTION(Server, Reliable)
	void ServerSetPhase(ERaceControllerPhase NewPhase);

	void PrepareForNewLap(float InServerStartTime);

	FOnStartLapCountdownSignature OnStartLapCountdown;
	FOnUpdateLapCountdownSignature OnUpdateLapCountdown;
	FOnUpdateRacePlayerStateSignature OnUpdateRacePlayerState;
};
