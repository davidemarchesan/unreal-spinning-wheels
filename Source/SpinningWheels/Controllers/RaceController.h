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
	void StartDrive();
	void StopDrive();
	void StartBrake();
	void StopBrake();
	void Turn(const FInputActionValue& Value);
	void StartTurbo();
	void StopTurbo();

protected:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;
	
};
