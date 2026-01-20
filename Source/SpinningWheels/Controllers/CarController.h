// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "CarController.generated.h"

class UInputMappingContext;
class UDriveInputConfig;
class ACar;
class AMainCamera;

/**
 * Debug use only
 */
UCLASS()
class SPINNINGWHEELS_API ACarController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDriveInputConfig> InputConfig;

	UPROPERTY(Category=Camera, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AMainCamera> CameraClass;

	TWeakObjectPtr<ACar> Car;
	TWeakObjectPtr<AMainCamera> MainCamera;

	void SetupCamera();

	/** Input Actions handler */
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

public:
	
	virtual void SetPawn(APawn* InPawn) override;
	
};
