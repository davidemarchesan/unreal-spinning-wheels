// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"

#include "Camera/CameraComponent.h"

AMainCamera::AMainCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;
}

void AMainCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCamera(DeltaSeconds);
}

void AMainCamera::SetupForCar()
{
	FollowingCar = Cast<ACar>(FollowingPawn);
	if (FollowingCar.IsValid())
	{
		CarMovementComponent = FollowingCar->GetCarMovementComponent();
		CameraMode = ECameraMode::CAMERAMODE_Car;
	}
}

void AMainCamera::UpdateCamera(float DeltaSeconds)
{
	switch (CameraMode)
	{
	default:
	case ECameraMode::CAMERAMODE_None:
		break;

	case ECameraMode::CAMERAMODE_Car:
		UpdateCameraForCar(DeltaSeconds);
		break;
	}
}

void AMainCamera::UpdateCameraForCar(float DeltaSeconds)
{
	if (FollowingCar.IsValid() == false || CarMovementComponent.IsValid() == false)
	{
		// todo: what are we going to do?
		UE_LOG(LogTemp, Warning, TEXT("AMainCamera::UpdateCameraForCar car or mov are null"));
		return;
	}

	const float VelocityRatio = CarMovementComponent->GetVelocityRatio();
	
	/** LOCATION */
	SetActorLocation(FollowingCar->GetActorLocation()); // Pivot
	if (UCameraComponent* Camera = GetCameraComponent())
	{
		FVector RelativeLocation = -FVector::ForwardVector * OffsetFromCar.X;
		RelativeLocation.X -= VelocityRatio * OffsetFromCarVelocityMultiplier; 
		RelativeLocation.Z = OffsetFromCar.Z;
		Camera->SetRelativeLocation(RelativeLocation);
	}

	const FRotator TargetRotation = FollowingCar->GetActorRotation();
	const FRotator CurrentRotation = GetActorRotation();

	/** ROTATION */
	const FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, RotationLerpSpeed * VelocityRatio);
	SetActorRotation(NewRotation);
	
}

void AMainCamera::SetPawn(APawn* InPawn)
{
	if (InPawn)
	{
		FollowingPawn = InPawn;

		if (FollowingPawn->IsA(ACar::StaticClass()))
		{
			SetupForCar();
		}
	}
}
