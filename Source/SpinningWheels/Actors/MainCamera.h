// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SpinningWheels/Pawns/Car.h"
#include "SpinningWheels/Pawns/Components/CarMovementComponent.h"
#include "MainCamera.generated.h"

class ACar;
class UCarMovementComponent;

class AEditorPawn;

enum ECameraMode : int
{
	CAMERAMODE_None,
	CAMERAMODE_Car,
	CAMERAMODE_Editor
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

	void SetupForEditor();
	TWeakObjectPtr<AEditorPawn> FollowingEditor;

	void UpdateCamera(float DeltaSeconds);
	void UpdateCameraIdle(float DeltaSeconds);
	void UpdateCameraForCar(float DeltaSeconds);
	void UpdateCameraForEditor(float DeltaSeconds);

protected:

	virtual void BeginPlay() override;

public:

	void SetPawn(APawn* InPawn);

	UPROPERTY(Category=Idle, EditAnywhere)
	FVector IdleLocation = FVector::ZeroVector;

	UPROPERTY(Category=Idle, EditAnywhere)
	FVector IdleLocationOffset;

	UPROPERTY(Category=Idle, EditAnywhere)
	float IdleRotationSpeed;

	UPROPERTY(Category=Car, EditAnywhere)
	FVector CarOffsetFromPawn;

	UPROPERTY(Category=Car, EditAnywhere)
	float CarOffsetFromPawnVelocityMultiplier;

	UPROPERTY(Category=Car, EditAnywhere)
	float CarRotationLerpSpeed;

	UPROPERTY(Category=Car, EditAnywhere)
	FRotator CarCameraRotation;

	UPROPERTY(Category=Editor, EditAnywhere)
	FVector EditorOffsetFromPawn;

	UPROPERTY(Category=Editor, EditAnywhere)
	FRotator EditorCameraRotation;
	
};
