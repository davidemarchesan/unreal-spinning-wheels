// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EditorPawn.generated.h"

UCLASS()
class SPINNINGWHEELS_API AEditorPawn : public APawn
{
	GENERATED_BODY()

public:
	AEditorPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	FVector MovementInputValue = FVector::ZeroVector;
	float RotationInputValue = 0.f;

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(Category=Movement, EditAnywhere)
	float MovementSpeed;

	UPROPERTY(Category=Movement, EditAnywhere)
	float RotationSpeed;

	void Move(FVector2D InputVector);
	void Rotate(FVector2D InputVector);
	
};
