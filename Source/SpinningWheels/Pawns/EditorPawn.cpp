// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorPawn.h"

AEditorPawn::AEditorPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEditorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEditorPawn::Move(FVector2D InputVector)
{
	MovementInputValue = FVector(InputVector.X, InputVector.Y, 0.0f);
}

void AEditorPawn::Rotate(FVector2D InputVector)
{
	RotationInputValue = -InputVector.Y;
}

void AEditorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalOffset(MovementInputValue * MovementSpeed * DeltaTime, false);
	AddActorLocalRotation(FRotator(0.f, RotationInputValue * RotationSpeed * DeltaTime, 0.f), false);
	
	MovementInputValue = FVector::ZeroVector;
	RotationInputValue = 0.f;

}

void AEditorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

