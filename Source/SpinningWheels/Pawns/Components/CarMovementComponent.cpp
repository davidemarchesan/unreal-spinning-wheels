// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"

void UCarMovementComponent::CalcAcceleration(float DeltaTime)
{
	if (AccelerationCurve == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UCarMovementComponent::CalcAcceleration() - No acceleration curve was provided"));
		return;
	}

	if (DriveInputValue > 0.f)
	{
		const float Alpha = IsSpeedZero() ? 0.f : Speed / MaxSpeed;
		const float CurveMultiplier = AccelerationCurve->GetFloatValue(Alpha);

		Acceleration = MaxAcceleration * CurveMultiplier * DriveInputValue;
	}
	else
	{
		Acceleration = 0.f;
	}
}

void UCarMovementComponent::CalcBrakeDeceleration(float DeltaTime)
{
	if (BrakeDecelerationCurve == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UCarMovementComponent::CalcBrakeDeceleration() - No brake deceleration curve was provided"));
		return;
	}

	if (BrakeInputValue > 0.f)
	{
		BrakeHoldTime += DeltaTime;
		const float Alpha = BrakeHoldTime >= MaxBrakeHoldTime ? 1.f : BrakeHoldTime / MaxBrakeHoldTime;
		const float CurveMultiplier = BrakeDecelerationCurve->GetFloatValue(Alpha);

		BrakeDeceleration = MaxBrakeDeceleration * CurveMultiplier * BrakeInputValue;
	}
	else
	{
		BrakeHoldTime = 0.f;
		BrakeDeceleration = 0.f;
	}
}

void UCarMovementComponent::CalcSpeed(float DeltaTime)
{
	Speed += Acceleration * DeltaTime;
	Speed -= BrakeDeceleration * DeltaTime;
	Speed = FMath::Clamp(Speed, 0.f, MaxSpeed);
}

void UCarMovementComponent::CalcRotation()
{
	AngularVelocity = FRotator(0.f, TurnInputValue * AngularSpeed, 0.f);
}

void UCarMovementComponent::ApplyForces(float DeltaTime)
{
	if (IsSpeedZero() == false)
	{
		Speed -= GroundFriction * DeltaTime;
	}
}

void UCarMovementComponent::ResetDriveInputValue()
{
	DriveInputValue = 0.f;
}

void UCarMovementComponent::ResetBrakeInputvalue()
{
	BrakeInputValue = 0.f;
}

void UCarMovementComponent::ResetTurnInputValue()
{
	TurnInputValue = 0.f;
}

bool UCarMovementComponent::IsSpeedZero()
{
	return FMath::IsNearlyZero(Speed, 0.0002f);
}

void UCarMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DeltaTime < 1e-6f)
	{
		return;
	}

	CalcAcceleration(DeltaTime);
	CalcBrakeDeceleration(DeltaTime);
	CalcSpeed(DeltaTime);

	CalcRotation();

	ApplyForces(DeltaTime);

	if (UpdatedComponent)
	{
		Velocity = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal() * Speed;
		UpdatedComponent->MoveComponent(
			Velocity * DeltaTime,
			UpdatedComponent->GetComponentRotation() + (AngularVelocity * DeltaTime),
			true
		);
	}

	ResetDriveInputValue();
	ResetBrakeInputvalue();
	ResetTurnInputValue();
}

void UCarMovementComponent::Drive()
{
	DriveInputValue = 1.f;
}

void UCarMovementComponent::Turn(FVector2D InputVector)
{
	TurnInputValue = InputVector.Y;

	// if (UpdatedComponent)
	// {
	// 	// InputVector.Y is always -1 or 1 (at the moment)
	// 	// todo: handle zeroes values
	// 	// todo: does grip influence how much i can turn? (i think so) (at any speed?)
	// 	UpdatedComponent->
	// 		SetWorldRotation(UpdatedComponent->GetComponentRotation() + FRotator(0.f, InputVector.Y, 0.f));
	// }
}

void UCarMovementComponent::Brake()
{
	BrakeInputValue = 1.f;
}
