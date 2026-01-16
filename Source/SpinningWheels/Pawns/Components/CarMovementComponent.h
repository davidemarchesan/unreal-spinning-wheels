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

	float DriveInputValue = 0.f;		// Is player pushing the drive button (float to handle decimal values in future?)

	void CalcAcceleration(float DeltaTime);
	void CalcSpeed();
	void ResetDriveInputValue();

	bool IsSpeedZero();

protected:

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Drive();
	void Turn(FVector2D InputVector);

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	float MaxSpeed;

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	float MaxAcceleration;

	UPROPERTY(Category=Drive, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> AccelerationCurve;
	
	float GetCurrentSpeed() const { return Speed; }
	float GetCurrentAcceleration() const { return Acceleration; }
	float GetDriveInputValue() const { return DriveInputValue; }
	
};
