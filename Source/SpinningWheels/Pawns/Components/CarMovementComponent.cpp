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
	float VelocitySpeed = Velocity.Length() + (Acceleration * SimulationConstants::TickFrequency) - (BrakeDeceleration * SimulationConstants::TickFrequency) - (
		GroundFriction * SimulationConstants::TickFrequency);
	VelocitySpeed = FMath::Clamp(VelocitySpeed, 0.f, MaxSpeed);

	switch (CarMode)
	{
	default:
	case ECarMode::CARMODE_Drive:
		{
			if (IsSurfaceSliding())
			{
				const FVector RotationDir = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal();
				const FVector SlidingDir = Velocity.GetSafeNormal();

				const float Diff = 1.f - (RotationDir | SlidingDir);
				if (FMath::IsNearlyZero(Diff, 0.0001f))
				{
					bSurfaceSliding = false;
				}
				else
				{
					VelocitySpeed -= (WallsSlideFriction * SimulationConstants::TickFrequency);
					VelocitySpeed = FMath::Max(VelocitySpeed, 0.f);
					Velocity = Velocity.GetSafeNormal() * VelocitySpeed;
				}
			}


			if (bSurfaceSliding == false)
			{
				Velocity = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal() * VelocitySpeed;
			}
			break;
		}

	case ECarMode::CARMODE_Slide:
		{
			FVector TargetDirection = UpdatedComponent->GetComponentRotation().Vector().GetSafeNormal();
			FVector CurrentDirection = Velocity.GetSafeNormal();
			const float Alpha = SlideLerpSpeed * SimulationConstants::TickFrequency;

			FVector NewDirection = FMath::Lerp(CurrentDirection, TargetDirection, Alpha);

			VelocitySpeed -= (GroundFriction * SlideGroundFrictionMultiplier * SimulationConstants::TickFrequency);

			Velocity = NewDirection * VelocitySpeed;
			break;
		}

	case ECarMode::CARMODE_Crash:
		{
			VelocitySpeed = Velocity.Length() - (GroundFriction * SimulationConstants::TickFrequency);
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
		BrakeHoldTime += SimulationConstants::TickFrequency;
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
		const float NewAngVel = FMath::Lerp(AngularVelocity.Yaw, 0.f, 0.8f * SimulationConstants::TickFrequency);
		AngularVelocity.Yaw = NewAngVel;
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

	AngularVelocity = FRotator(
		0.f, CurrentSimulationFrame.TurnInputValue * AngularSpeedMultiplier * CurveMultiplier,
		0.f);
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
		0.f, CurrentSimulationFrame.TurnInputValue * AngularSpeedMultiplier * SlideAngularSpeedMultiplier *
		CurveMultiplier, 0.f);
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
	const FVector VelocityNormalized = Velocity.GetSafeNormal();

	const float Dot = FVector::DotProduct(Hit.ImpactNormal, -VelocityNormalized);
	const float ImpactAngle = FMath::RadiansToDegrees(FMath::Acos(Dot));

	const float AngleDiff = 90.f - ImpactAngle;
	// UE_LOG(LogTemp, Warning, TEXT("Speed: %f | Dot: %f | Angle: %f | Diff: %f"), Velocity.Length(), Dot, ImpactAngle,
	//        Diff);

	if (IsSurfaceSliding() || AngleDiff <= MaxSurfaceSlideAngle || Velocity.Length() <= MinSpeedToBounce)
	{
		bSurfaceSliding = true;

		// Slide along surface
		FVector SlideVector = VelocityNormalized - FVector::DotProduct(
				VelocityNormalized, Hit.ImpactNormal) * Hit.
			ImpactNormal;

		Velocity = SlideVector * Velocity.Length() * WallsBounceForce;
		Velocity += Hit.ImpactNormal * 0.1f; // Just get a little be safe will ya?

		const FRotator TargetRotation = SlideVector.Rotation();
		const FRotator Rotation = FMath::RInterpTo(UpdatedComponent->GetComponentRotation(), TargetRotation,
		                                           SimulationConstants::TickFrequency, WallsSlideRotationInterpSpeed);

		SafeMoveUpdatedComponent(
			Velocity * SimulationConstants::TickFrequency,
			Rotation,
			true,
			Hit
		);

		SetMode(ECarMode::CARMODE_Drive);
	}
	else
	{
		// Bounce back
		FVector BounceVector = VelocityNormalized - 2.f * FVector::DotProduct(
				VelocityNormalized, Hit.ImpactNormal) * Hit.
			ImpactNormal;

		Velocity = BounceVector * Velocity.Length() * WallsBounceForce;

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

		SetMode(ECarMode::CARMODE_Crash);
	}

	// END Angle between normal and velocity
}

void UCarMovementComponent::SimulateMovement(FSimulationFrame SimulationFrame)
{
	CurrentSimulationFrame = SimulationFrame;

	if (CarMode == CARMODE_Drive)
	{
		if (IsTurning() && IsBraking() && IsSurfaceSliding() == false)
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

		// It's basically idle but somehow in slide (may comes from crash)
		if (FMath::IsNearlyZero(Velocity.Length(), 0.4f) && IsAccelerating() == false)
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
		if (IsSurfaceSliding())
		{

			// Slide along surface
			// and rotate along surface
			const FRotator TargetRotation = Velocity.GetSafeNormal().Rotation();
			const FRotator Rotation = FMath::RInterpTo(UpdatedComponent->GetComponentRotation(), TargetRotation,
												   SimulationConstants::TickFrequency, WallsSlideRotationInterpSpeed);
			
			UpdatedComponent->MoveComponent(
				Velocity * SimulationConstants::TickFrequency,
				Rotation,
				true,
				&Hit
			);
			
		}
		else
		{
			UpdatedComponent->MoveComponent(
				Velocity * SimulationConstants::TickFrequency,
				UpdatedComponent->GetComponentRotation() + (AngularVelocity * SimulationConstants::TickFrequency),
				true,
				&Hit
			);
		}

		if (CarMode == ECarMode::CARMODE_Crash)
		{
			if (FMath::IsNearlyZero(AngularVelocity.Yaw, 85.f))
			{
				// Retake control of the car
				SetMode(ECarMode::CARMODE_Slide);
			}
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
