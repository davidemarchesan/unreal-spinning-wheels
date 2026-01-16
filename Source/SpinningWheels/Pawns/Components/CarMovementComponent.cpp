// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"

void UCarMovementComponent::CalcAcceleration(float DeltaTime)
{
	if (AccelerationCurve == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UCarMovementComponent::CalcAcceleration() - No acceleration curve was provided"));
		return;
	}

	const float Alpha = FMath::IsNearlyZero(Speed, 0.002f) ? 0.f : Speed / MaxSpeed;
	const float CurveMultiplier = AccelerationCurve->GetFloatValue(Alpha);

	Acceleration = MaxAcceleration * CurveMultiplier * DriveInputValue * DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("a %f maxa %f cm %f div %f"), Acceleration, MaxAcceleration, CurveMultiplier, DriveInputValue);
	
}

void UCarMovementComponent::CalcSpeed()
{
	Speed += Acceleration;
	Speed = FMath::Clamp(Speed, 0.f, MaxSpeed);
}

void UCarMovementComponent::ResetDriveInputValue()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetDriveInputValue()"));
	DriveInputValue = 0.f;
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
	CalcSpeed();
	
	// Apply drag
	if (IsSpeedZero() == false)
	{
		// Speed -= 1.f; // simple drag to slow down moving car
	}

	if (UpdatedComponent)
	{
		Velocity = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal() * Speed;
		UpdatedComponent->MoveComponent(Velocity * DeltaTime, UpdatedComponent->GetComponentRotation(), true);
	}

	// ResetDriveInputValue();
	
}

void UCarMovementComponent::Drive()
{
	DriveInputValue = 1.f;
}

void UCarMovementComponent::Turn(FVector2D InputVector)
{
	// Avoid to turn the car while not moving
	if (FMath::IsNearlyZero(Speed, 0.002f))
	{
		return;
	}
	
	if (UpdatedComponent)
	{
		// InputVector.Y is always -1 or 1 (at the moment)
		// todo: handle zeroes values
		// todo: does grip influence how much i can turn? (i think so) (at any speed?)
		UpdatedComponent->SetWorldRotation(UpdatedComponent->GetComponentRotation() + FRotator(0.f, InputVector.Y, 0.f));
	}
}
