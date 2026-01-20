// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CarMovementComponent.generated.h"

enum ECarMode : int
{
	CARMODE_Drive,
	CARMODE_Slide,
	CARMODE_Reverse,
	CARMODE_Crash,
	CARMODE_Fly
};

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API UCarMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

private:

	TEnumAsByte<ECarMode> CarMode = ECarMode::CARMODE_Fly;
	void SetMode(ECarMode NewMode);

	void StartDrive();
	void StartSlide();

	float Speed = 0.f;									// (Deprecated: use velocity.length) Current speed
	float Acceleration = 0.f;							// Current acceleration
	float BrakeDeceleration = 0.f;						// Current deceleration given by break
	FRotator AngularVelocity = FRotator::ZeroRotator;	// Current rotation angular velocity (turning)

	float DriveInputValue = 0.f;						// Is player pushing the drive button (float to handle decimal values in future?)
	float BrakeInputValue = 0.f;						// Is player pushing the brake button
	float TurnInputValue = 0.f;							// Is player turning (usually between -1 and 1)
	float TurboInputValue = 0.f;						// Is player pushing the turbo button

	float BrakeStartTime = 0.f;
	float BrakeHoldTime = 0.f;

	const float Gravity = 9.8f;
	bool bGrounded = false;

	void CalcVelocity(float DeltaTime);
	void CalcAcceleration(float DeltaTime);
	void CalcBrakeDeceleration(float DeltaTime);

	void CalcRotation(float DeltaTime);
	void CalcRotationDrive();
	void CalcRotationSlide();
	
	void ResetDriveInputValue();
	void ResetBrakeInputValue();
	void ResetTurnInputValue();
	void ResetTurboInputValue();

	bool IsSpeedZero();
	bool IsAccelerating();
	bool IsBraking();
	bool IsTurning();
	bool IsUsingTurbo();

	bool IsGrounded() const { return bGrounded; }

	void HandleCrash(float DeltaTime, FHitResult& Hit);

protected:

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Drive();
	void Turn(FVector2D InputVector);
	void Brake();
	void Turbo();

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
	float WallsBounceForce;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float CrashSpinMultiplier;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float GravityScale;

	ECarMode GetCarMode() const { return CarMode; }
	
	float GetSpeed() const { return Speed; }
	FVector GetVelocity() const { return Velocity; }
	float GetAcceleration() const { return Acceleration; }
	float GetBrakeDeceleration() const { return BrakeDeceleration; }
	FRotator GetAngularVelocity() const { return AngularVelocity; }
	
	float GetDriveInputValue() const { return DriveInputValue; }
	float GetBrakeInputValue() const { return BrakeInputValue; }
	float GetTurnInputValue() const { return TurnInputValue; }
	
};
