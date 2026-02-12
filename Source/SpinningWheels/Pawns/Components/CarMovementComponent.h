// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SpinningWheels/Core/Simulation.h"
#include "CarMovementComponent.generated.h"

enum ECarMode : int
{
	CARMODE_Drive,
	CARMODE_Slide,
	CARMODE_Reverse,
	CARMODE_Crash,
	CARMODE_Fly
};


class ARacePlayerState;

UCLASS()
class SPINNINGWHEELS_API UCarMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

private:

	// Begin Deterministic physics
	FSimulationFrame CurrentSimulationFrame;
	// End Deterministic physics

	TEnumAsByte<ECarMode> CarMode = ECarMode::CARMODE_Fly;
	void SetMode(ECarMode NewMode);

	int8 Engine = 0;

	void StartDrivePhysics();
	void StartSlidePhysics();

	float Acceleration = 0.f;							// Current acceleration
	float BrakeDeceleration = 0.f;						// Current deceleration given by break
	FRotator AngularVelocity = FRotator::ZeroRotator;	// Current rotation angular velocity (turning)
	
	float BrakeHoldTime = 0.f;

	const float Gravity = 9.8f;
	bool bGrounded = false;

	bool bSurfaceSliding = false;

	void CalcVelocity(float DeltaTime);
	void CalcAcceleration(float DeltaTime);
	void CalcBrakeDeceleration(float DeltaTime);

	void CalcRotation(float DeltaTime);
	void CalcRotationDrive();
	void CalcRotationSlide();
	
	bool IsSpeedZero();
	bool IsAccelerating();
	bool IsBraking();
	bool IsTurning();
	bool IsUsingTurbo();

	bool IsGrounded() const { return bGrounded; }
	bool IsSurfaceSliding() const { return bSurfaceSliding; }

	void HandleCrash(float DeltaTime, FHitResult& Hit);

protected:

public:

	void SimulateMovement(FSimulationFrame SimulationFrame);

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	float MaxSpeed;

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	float MaxAcceleration;

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> AccelerationCurve;

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	float TurboAcceleration;

	UPROPERTY(Category=Brake, EditAnywhere, BlueprintReadOnly)
	float MaxBrakeDeceleration;

	UPROPERTY(Category=Brake, EditAnywhere, BlueprintReadOnly, meta=(ToolTip="Time in X axis in brake deceleration curve field."))
	float MaxBrakeHoldTime;

	UPROPERTY(Category=Brake, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> BrakeDecelerationCurve;

	UPROPERTY(Category=Turn, EditAnywhere, BlueprintReadOnly)
	float AngularSpeedMultiplier;

	UPROPERTY(Category=Turn, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> AngularSpeedCurve;

	UPROPERTY(Category=Slide, EditAnywhere, BlueprintReadOnly, meta=(Units="%"))
	float MinSpeedToSlide;

	UPROPERTY(Category=Slide, EditAnywhere, BlueprintReadOnly)
	float SlideAngularSpeedMultiplier;

	UPROPERTY(Category=Slide, EditAnywhere, BlueprintReadOnly)
	float SlideGroundFrictionMultiplier;

	UPROPERTY(Category=Slide, EditAnywhere, BlueprintReadOnly)
	float SlideLerpSpeed;
	
	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float GroundFriction;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float MaxSurfaceSlideAngle;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float WallsSlideFriction;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float WallsSlideRotationInterpSpeed;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float MinSpeedToBounce;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float WallsBounceForce;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float CrashSpinMultiplier;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float GravityScale;

	ECarMode GetCarMode() const { return CarMode; }
	
	FVector GetVelocity() const { return Velocity; }
	float GetVelocityMagnitude() const { return Velocity.Size(); }
	float GetVelocityRatio() const { return Velocity.Length() / MaxSpeed; }
	
	float GetAcceleration() const { return Acceleration; }
	float GetBrakeDeceleration() const { return BrakeDeceleration; }
	FRotator GetAngularVelocity() const { return AngularVelocity; }
	
};
