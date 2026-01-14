// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Car.generated.h"

class UArrowComponent;
class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class SPINNINGWHEELS_API ACar : public APawn
{
	GENERATED_BODY()

public:

	ACar();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

#if WITH_EDITORONLY_DATA
	
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
	
#endif


protected:

	virtual void BeginPlay() override;

public:	

};
