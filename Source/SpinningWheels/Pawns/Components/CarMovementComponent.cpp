// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"

void UCarMovementComponent::SetMode(ECarMode NewMode)
{
	CarMode = NewMode;
	switch (CarMode)
	{
	default:
	case ECarMode::CARMODE_Drive:
		StartDrive();
		break;
	case ECarMode::CARMODE_Slide:
		StartSlide();
		break;
	}
}

void UCarMovementComponent::StartDrive()
{
}

void UCarMovementComponent::StartSlide()
{
}

void UCarMovementComponent::CalcVelocity(float DeltaTime)
{
	switch (CarMode)
	{
	default:
	case ECarMode::CARMODE_Drive:
		Velocity = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal() * Speed;
		break;

	case ECarMode::CARMODE_Slide:

		FVector TargetDirection = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal();
		FVector CurrentDirection = Velocity.GetSafeNormal();
		const float Alpha = SlideLerpSpeed * DeltaTime;

		FVector NewDirection = FMath::Lerp(CurrentDirection, TargetDirection, Alpha);

		Velocity = NewDirection * Speed;
		break;
	}
}

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
	// Avoid to rotate car when not moving
	if (IsSpeedZero())
	{
		AngularVelocity = FRotator::ZeroRotator;
		return;
	}

	switch (CarMode)
	{
	default:
	case ECarMode::CARMODE_Drive:
		CalcRotationDrive();
		break;

	case ECarMode::CARMODE_Slide:
		CalcRotationSlide();
		break;
	}
}

void UCarMovementComponent::CalcRotationDrive()
{
	float CurveMultiplier = 1.f; // Default
	if (AngularSpeedCurve)
	{
		const float Alpha = IsSpeedZero() ? 0.f : Speed / MaxSpeed;
		CurveMultiplier = AngularSpeedCurve->GetFloatValue(Alpha);
	}

	AngularVelocity = FRotator(0.f, TurnInputValue * AngularSpeedMultiplier * CurveMultiplier, 0.f);
}

void UCarMovementComponent::CalcRotationSlide()
{
	float CurveMultiplier = 1.f; // Default
	if (AngularSpeedCurve)
	{
		const float Alpha = IsSpeedZero() ? 0.f : Speed / MaxSpeed;
		CurveMultiplier = AngularSpeedCurve->GetFloatValue(Alpha);
	}
	
	AngularVelocity = FRotator(0.f, TurnInputValue * AngularSpeedMultiplier * SlideAngularSpeedMultiplier * CurveMultiplier, 0.f);
}

void UCarMovementComponent::ApplyForces(float DeltaTime)
{
	if (IsSpeedZero())
	{
		return;
	}

	switch (CarMode)
	{
	default:
	case ECarMode::CARMODE_Drive:
		Speed -= GroundFriction * DeltaTime;
		break;

	case ECarMode::CARMODE_Slide:
		Speed -= GroundFriction * SlideGroundFrictionMultiplier * DeltaTime;
		break;
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

bool UCarMovementComponent::IsAccelerating()
{
	return Acceleration > 0.f;
}

bool UCarMovementComponent::IsBraking()
{
	return BrakeInputValue > 0.f;
}

bool UCarMovementComponent::IsTurning()
{
	return TurnInputValue != 0.f;
}

void UCarMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DeltaTime < 1e-6f)
	{
		return;
	}

	if (CarMode == CARMODE_Drive)
	{
		if (IsTurning() && IsBraking())
		{
			SetMode(ECarMode::CARMODE_Slide);
		}
	}
	else if (CarMode == ECarMode::CARMODE_Slide)
	{
		const FVector Difference = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal() - Velocity.
			GetSafeNormal();

		if (IsBraking() == false && Difference.IsNearlyZero(0.1f))
		{
			SetMode(ECarMode::CARMODE_Drive);
		}
	}

	CalcAcceleration(DeltaTime);
	CalcBrakeDeceleration(DeltaTime);
	CalcSpeed(DeltaTime);

	CalcRotation();

	ApplyForces(DeltaTime);

	if (UpdatedComponent)
	{
		CalcVelocity(DeltaTime);

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
}

void UCarMovementComponent::Brake()
{
	BrakeInputValue = 1.f;
}
