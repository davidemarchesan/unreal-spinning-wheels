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

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ARaceController : public APlayerController
{
	GENERATED_BODY()

public:

	ARaceController();

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
	
public:

	ARaceGameMode* GetRaceGameMode();
	ARaceGameState* GetRaceGameState();
	ARacePlayerState* GetRacePlayerState();
	
};
