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
		SetActorRotation(FollowingCar->GetActorRotation());
		CameraMode = ECameraMode::CAMERAMODE_Car;
	}

	if (UCameraComponent* Camera = GetCameraComponent())
	{
		Camera->SetRelativeRotation(CarCameraRotation);
	}
}

void AMainCamera::SetupForEditor()
{
	FollowingEditor = Cast<AEditorPawn>(FollowingPawn);
	if (FollowingEditor.IsValid())
	{
		CameraMode = ECameraMode::CAMERAMODE_Editor;
	}

	if (UCameraComponent* Camera = GetCameraComponent())
	{
		Camera->SetRelativeRotation(EditorCameraRotation);
	}
}

void AMainCamera::UpdateCamera(float DeltaSeconds)
{
	if (HasAuthority() == false)
	{
		return;
	}
	
	switch (CameraMode)
	{
	default:
	case ECameraMode::CAMERAMODE_None:
		UpdateCameraIdle(DeltaSeconds);
		break;

	case ECameraMode::CAMERAMODE_Car:
		UpdateCameraForCar(DeltaSeconds);
		break;

	case ECameraMode::CAMERAMODE_Editor:
		UpdateCameraForEditor(DeltaSeconds);
		break;
	}
}

void AMainCamera::UpdateCameraIdle(float DeltaSeconds)
{
	SetActorRotation(FRotator(0.f, GetActorRotation().Yaw + (1.f * IdleRotationSpeed * DeltaSeconds), 0.f));
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
		FVector RelativeLocation = -FVector::ForwardVector * CarOffsetFromPawn.X;
		RelativeLocation.X -= VelocityRatio * CarOffsetFromPawnVelocityMultiplier;
		RelativeLocation.Z = CarOffsetFromPawn.Z;
		Camera->SetRelativeLocation(RelativeLocation);
	}

	const FRotator TargetRotation = FollowingCar->GetActorRotation();
	const FRotator CurrentRotation = GetActorRotation();

	/** ROTATION */
	const FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, CarRotationLerpSpeed * VelocityRatio);
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
		FVector RelativeLocation = -FVector::ForwardVector * EditorOffsetFromPawn.X;
		RelativeLocation.Z = EditorOffsetFromPawn.Z;
		Camera->SetRelativeLocation(RelativeLocation);
	}

	SetActorRotation(FollowingEditor->GetActorRotation());
}

void AMainCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(IdleLocation); // Pivot
	if (UCameraComponent* Camera = GetCameraComponent())
	{
		FVector RelativeLocation = -FVector::ForwardVector * IdleLocationOffset.X;
		RelativeLocation.Z = IdleLocationOffset.Z;
		Camera->SetRelativeLocation(RelativeLocation);
	}
	
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
