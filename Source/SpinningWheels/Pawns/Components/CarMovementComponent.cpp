// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"

#include "SpinningWheels/PlayerStates/RacePlayerState.h"

void UCarMovementComponent::SetMode(ECarMode NewMode)
{
	CarMode = NewMode;
	switch (CarMode)
	{
	default:
	case ECarMode::CARMODE_Drive:
		StartDrivePhysics();
		break;
	case ECarMode::CARMODE_Slide:
		StartSlidePhysics();
		break;
	}
}

void UCarMovementComponent::StartDrivePhysics()
{
}

void UCarMovementComponent::StartSlidePhysics()
{
}

void UCarMovementComponent::CalcVelocity(float DeltaTime)
{
	float VelocitySpeed = Velocity.Length() + (Acceleration * DeltaTime) - (BrakeDeceleration * DeltaTime) - (
		GroundFriction * DeltaTime);
	VelocitySpeed = FMath::Clamp(VelocitySpeed, 0.f, MaxSpeed);

	switch (CarMode)
	{
	default:
	case ECarMode::CARMODE_Drive:
		{
			Velocity = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal() * VelocitySpeed;
			break;
		}

	case ECarMode::CARMODE_Slide:
		{
			FVector TargetDirection = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal();
			FVector CurrentDirection = Velocity.GetSafeNormal();
			const float Alpha = SlideLerpSpeed * DeltaTime;

			FVector NewDirection = FMath::Lerp(CurrentDirection, TargetDirection, Alpha);

			VelocitySpeed -= (GroundFriction * SlideGroundFrictionMultiplier * DeltaTime);

			Velocity = NewDirection * VelocitySpeed;
			break;
		}

	case ECarMode::CARMODE_Crash:
		{
			VelocitySpeed = Velocity.Length() - (GroundFriction * DeltaTime);
			Velocity = Velocity.GetSafeNormal() * VelocitySpeed;
			break;
		}
	case ECarMode::CARMODE_Fly:
		{
			Velocity = FVector::DownVector * Gravity * GravityScale;
			break;
		}
	}
}

void UCarMovementComponent::CalcAcceleration(float DeltaTime)
{
	if (AccelerationCurve == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UCarMovementComponent::CalcAcceleration() - No acceleration curve was provided"));
		return;
	}

	if (CurrentSimulationFrame.DriveInputValue > 0.f)
	{
		const float Alpha = IsSpeedZero() ? 0.f : Velocity.Length() / MaxSpeed;
		const float CurveMultiplier = AccelerationCurve->GetFloatValue(Alpha);

		Acceleration = MaxAcceleration * CurveMultiplier * CurrentSimulationFrame.DriveInputValue;

		if (IsUsingTurbo() == true)
		{
			Acceleration += TurboAcceleration;
		}
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

	if (CurrentSimulationFrame.BrakeInputValue > 0.f)
	{
		BrakeHoldTime += DeltaTime;
		const float Alpha = BrakeHoldTime >= MaxBrakeHoldTime ? 1.f : BrakeHoldTime / MaxBrakeHoldTime;
		const float CurveMultiplier = BrakeDecelerationCurve->GetFloatValue(Alpha);

		BrakeDeceleration = MaxBrakeDeceleration * CurveMultiplier * CurrentSimulationFrame.BrakeInputValue;
	}
	else
	{
		BrakeHoldTime = 0.f;
		BrakeDeceleration = 0.f;
	}
}

void UCarMovementComponent::CalcRotation(float DeltaTime)
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

	case ECarMode::CARMODE_Crash:
		float N = FMath::Lerp(AngularVelocity.Yaw, 0.f, 0.8f * DeltaTime);
		AngularVelocity.Yaw = N;
		// todo: retake control of the car?
		break;
	}
}

void UCarMovementComponent::CalcRotationDrive()
{
	float CurveMultiplier = 1.f; // Default
	if (AngularSpeedCurve)
	{
		const float Alpha = IsSpeedZero() ? 0.f : Velocity.Length() / MaxSpeed;
		CurveMultiplier = AngularSpeedCurve->GetFloatValue(Alpha);
	}

	AngularVelocity = FRotator(0.f, CurrentSimulationFrame.TurnInputValue * AngularSpeedMultiplier * CurveMultiplier, 0.f);
}

void UCarMovementComponent::CalcRotationSlide()
{
	float CurveMultiplier = 1.f; // Default
	if (AngularSpeedCurve)
	{
		const float Alpha = IsSpeedZero() ? 0.f : Velocity.Length() / MaxSpeed;
		CurveMultiplier = AngularSpeedCurve->GetFloatValue(Alpha);
	}

	AngularVelocity = FRotator(
		0.f, CurrentSimulationFrame.TurnInputValue * AngularSpeedMultiplier * SlideAngularSpeedMultiplier * CurveMultiplier, 0.f);
}

bool UCarMovementComponent::IsSpeedZero()
{
	return FMath::IsNearlyZero(Velocity.Length(), 0.0002f);
}

bool UCarMovementComponent::IsAccelerating()
{
	return Acceleration > 0.f;
}

bool UCarMovementComponent::IsBraking()
{
	return CurrentSimulationFrame.BrakeInputValue == 1;
}

bool UCarMovementComponent::IsTurning()
{
	return CurrentSimulationFrame.TurnInputValue != 0;
}

bool UCarMovementComponent::IsUsingTurbo()
{
	return CurrentSimulationFrame.TurboInputValue == 1;
}

void UCarMovementComponent::HandleCrash(float DeltaTime, FHitResult& Hit)
{
	// Angle between normal and velocity
	// Dont need it but leave it here
	// const FVector VelocityNormalized = Velocity.GetSafeNormal();
	//
	// const float Dot = FVector::DotProduct(Hit.ImpactNormal, -VelocityNormalized);
	// const float ImpactAngle = FMath::RadiansToDegrees(FMath::Acos(Dot));
	// UE_LOG(LogTemp, Warning, TEXT("Dot %f | Angle: %f"), Dot, ImpactAngle);
	//
	// const float MinSpeedToCrash = 800.f;
	// const float MaxSlideAngle = 15.f;
	//
	// const float Diff = 90.f - ImpactAngle;
	// END Angle between normal and velocity

	const FVector VelocityNormalized = Velocity.GetSafeNormal();
	FVector BounceVector = VelocityNormalized - 2.f * FVector::DotProduct(
			VelocityNormalized, Hit.ImpactNormal) * Hit.
		ImpactNormal;

	Velocity = BounceVector * Velocity.Length() * WallsBounceForce;
	// UE_LOG(LogTemp, Warning, TEXT("Velocity after bounce %s"), *Velocity.ToString());

	// Rotation impulse
	// Detect side of the impact
	FVector ImpactVector = Hit.ImpactPoint - GetActorLocation();
	ImpactVector.Normalize();

	// We use Dot product to detect where the impact has been relative to car center (Front or Behind? Left or Right?)
	const float ForwardVectorDot = FVector::DotProduct(ImpactVector, GetForwardVector());
	const float RightVectorDot = FVector::DotProduct(ImpactVector, GetOwner()->GetActorRightVector());

	// Initiate spin; +1 = Clockwise
	if (RightVectorDot > 0.f)
	{
		// Right
		AngularVelocity = FRotator(0.f, 1.f * Velocity.Length() * CrashSpinMultiplier, 0.f);
	}
	else
	{
		// Left
		AngularVelocity = FRotator(0.f, -1.f * Velocity.Length() * CrashSpinMultiplier, 0.f);
	}

	// UE_LOG(LogTemp, Warning, TEXT("Angular Velocity after bounce %s"), *AngularVelocity.ToString());

	SetMode(ECarMode::CARMODE_Crash);
}

void UCarMovementComponent::SimulateMovement(FSimulationFrame SimulationFrame)
{
	CurrentSimulationFrame = SimulationFrame;
	
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

	CalcAcceleration(SimulationConstants::TickFrequency);
	CalcBrakeDeceleration(SimulationConstants::TickFrequency);
	CalcRotation(SimulationConstants::TickFrequency);

	if (UpdatedComponent)
	{
		CalcVelocity(SimulationConstants::TickFrequency);

		FHitResult Hit;
		UpdatedComponent->MoveComponent(
			Velocity * SimulationConstants::TickFrequency,
			UpdatedComponent->GetComponentRotation() + (AngularVelocity * SimulationConstants::TickFrequency),
			true,
			&Hit
		);

		if (Hit.bStartPenetrating)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Hit StartPenetrating"));
		}

		if (Hit.bBlockingHit)
		{
			// todo: refactor ground when we do ramps, jump, and flying
			if (FMath::IsNearlyZero(1.f - Hit.ImpactNormal.Z, 0.002f))
			{
				if (CarMode == CARMODE_Fly)
				{
					Velocity = FVector::ZeroVector;
					SetMode(ECarMode::CARMODE_Drive);
				}
			}
			else
			{
				HandleCrash(SimulationConstants::TickFrequency, Hit);
			}
		}
	}

}
