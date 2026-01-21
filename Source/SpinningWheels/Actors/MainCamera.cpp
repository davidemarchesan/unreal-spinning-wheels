// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"

#include "Camera/CameraComponent.h"
#include "SpinningWheels/Pawns/EditorPawn.h"

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

void AMainCamera::SetupForEditor()
{
	FollowingEditor = Cast<AEditorPawn>(FollowingPawn);
	if (FollowingEditor.IsValid())
	{
		CameraMode = ECameraMode::CAMERAMODE_Editor;
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

	case ECameraMode::CAMERAMODE_Editor:
		UpdateCameraForEditor(DeltaSeconds);
		break;
	}
}

void AMainCamera::UpdateCameraForCar(float DeltaSeconds)
{
	if (FollowingCar.IsValid() == false || CarMovementComponent.IsValid() == false)
	{
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

void AMainCamera::UpdateCameraForEditor(float DeltaSeconds)
{
	if (FollowingEditor.IsValid() == false)
	{
		return;
	}

	SetActorLocation(FollowingEditor->GetActorLocation());
	if (UCameraComponent* Camera = GetCameraComponent())
	{
		FVector RelativeLocation = -FVector::ForwardVector * OffsetFromEditor.X;
		RelativeLocation.Z = OffsetFromEditor.Z;
		Camera->SetRelativeLocation(RelativeLocation);
	}

	SetActorRotation(FollowingEditor->GetActorRotation());
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

		if (FollowingPawn->IsA(AEditorPawn::StaticClass()))
		{
			SetupForEditor();
		}
	}
}
