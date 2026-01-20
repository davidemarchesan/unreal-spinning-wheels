// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/Pawns/Components/CarMovementComponent.h"
#include "MainCamera.generated.h"

class ACar;
class UCarMovementComponent;

enum ECameraMode : int
{
	CAMERAMODE_None,
	CAMERAMODE_Car
};

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API AMainCamera : public ACameraActor
{
	GENERATED_BODY()

public:

	AMainCamera();

	virtual void Tick(float DeltaSeconds) override;

private:

	TEnumAsByte<ECameraMode> CameraMode = ECameraMode::CAMERAMODE_None;

	TWeakObjectPtr<APawn> FollowingPawn;
	
	void SetupForCar();
	TWeakObjectPtr<ACar> FollowingCar;
	TWeakObjectPtr<UCarMovementComponent> CarMovementComponent;

	void UpdateCamera(float DeltaSeconds);
	void UpdateCameraForCar(float DeltaSeconds);

protected:

public:

	void SetPawn(APawn* InPawn);

	UPROPERTY(Category=Car, EditAnywhere)
	FVector OffsetFromCar;

	UPROPERTY(Category=Car, EditAnywhere)
	float OffsetFromCarVelocityMultiplier;

	UPROPERTY(Category=Car, EditAnywhere)
	float RotationLerpSpeed;
	
};
