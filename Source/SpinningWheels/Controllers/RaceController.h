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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartLapCountdownSignature, float, Seconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateLapCountdownSignature, int32, Seconds);

UCLASS()
class SPINNINGWHEELS_API ARaceController : public APlayerController
{
	GENERATED_BODY()

public:

	ARaceController();

	virtual void Tick(float DeltaSeconds) override;

private:

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

	void SetupDriveInputBindings();
	void SetupCamera();

	bool bCameraInitialized = false;

	UPROPERTY(Replicated)
	ERaceControllerPhase Phase = ERaceControllerPhase::RCP_Respawning;
	
	UPROPERTY(Replicated)
	float ServerStartDriveTime = 0.f;

	void StartDriveProcedure(float DeltaSeconds);
	int32 StartDriveSecondsRemaining = 0;

	void StartLap();

	/** Input Actions handler - Drive */
	void InputStartDrive();
	void InputStopDrive();
	void InputStartBrake();
	void InputStopBrake();
	void InputTurn(const FInputActionValue& Value);
	void InputStartTurbo();
	void InputStopTurbo();
	void InputCancelLap();
	
	UFUNCTION(Server, Reliable)
	void ServerCancelLap();

	/** Debug */
	void Debug();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

	ARaceGameMode* GetRaceGameMode();
	ARaceGameState* GetRaceGameState();
	ARacePlayerState* GetRacePlayerState();
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPhase(ERaceControllerPhase NewPhase);
	
	void PrepareForNewLap(float InServerStartTime);

	FOnStartLapCountdownSignature OnStartLapCountdown;
	FOnUpdateLapCountdownSignature OnUpdateLapCountdown;
	
};
