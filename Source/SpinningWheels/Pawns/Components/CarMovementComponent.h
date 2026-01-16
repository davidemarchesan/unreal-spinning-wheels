// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CarMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API UCarMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

private:

	float Speed = 0.f;					// Current speed
	float Acceleration = 0.f;			// Current acceleration
	float BrakeDeceleration = 0.f;		// Current deceleration given by break

	float DriveInputValue = 0.f;		// Is player pushing the drive button (float to handle decimal values in future?)
	float BrakeInputValue = 0.f;		// Is player pushing the brake button

	float BrakeStartTime = 0.f;
	float BrakeHoldTime = 0.f;

	void CalcAcceleration(float DeltaTime);
	void CalcBrakeDeceleration(float DeltaTime);
	void CalcSpeed();

	void ApplyForces(float DeltaTime);
	
	void ResetDriveInputValue();
	void ResetBrakeInputvalue();

	bool IsSpeedZero();

protected:

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Drive();
	void Turn(FVector2D InputVector);
	void Brake();

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	float MaxSpeed;

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	float MaxAcceleration;

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> AccelerationCurve;

	UPROPERTY(Category=Brake, EditAnywhere, BlueprintReadOnly)
	float MaxBrakeDeceleration;

	UPROPERTY(Category=Brake, EditAnywhere, BlueprintReadOnly, meta=(ToolTip="Time in X axis in brake deceleration curve field."))
	float MaxBrakeHoldTime;

	UPROPERTY(Category=Brake, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> BrakeDecelerationCurve;

	UPROPERTY(Category=Forces, EditAnywhere, BlueprintReadOnly)
	float GroundFriction;
	
	float GetCurrentSpeed() const { return Speed; }
	float GetCurrentAcceleration() const { return Acceleration; }
	float GetDriveInputValue() const { return DriveInputValue; }
	
};
