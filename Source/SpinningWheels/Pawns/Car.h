// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Car.generated.h"

class UArrowComponent;
class USkeletalMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UCarMovementComponent;

UCLASS()
class SPINNINGWHEELS_API ACar : public APawn
{
	GENERATED_BODY()

public:
	
	ACar(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(Category=Car, VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCarMovementComponent> CarMovementComponent;

	bool bDrive = false;
	bool bBrake = false;
	bool bTurn = false;
	bool bTurbo = false;
	
	float TurboMaxBattery = 1.f;
	float TurboCurrentBattery = 1.f;

	void ConsumeTurbo(float DeltaTime);
	

	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialLights;
	
	static const int32 MaterialIndexLights = 6;

	void UpdateLightsBehavior(float InIntensity, FLinearColor InColor, float InFlashing);
	void StopLights();

#if WITH_EDITORONLY_DATA
	
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
	
#endif


protected:

	virtual void BeginPlay() override;

public:

	UCarMovementComponent* GetCarMovementComponent() const { return CarMovementComponent; }

	void StartDrive();
	void StopDrive();
	void StartBrake();
	void StopBrake();
	void Turn(FVector2D InputVector);
	void StartTurbo();
	void StopTurbo();
	void ToggleTurbo();

	UPROPERTY(Category=Turbo, EditAnywhere)
	float TurboConsumption;

	UPROPERTY(Category=Lights, EditAnywhere)
	FLinearColor LightsColorOnTurbo;
	
	UPROPERTY(Category=Lights, EditAnywhere)
	float LightsFlashingOnTurbo;

	UPROPERTY(Category=Lights, EditAnywhere)
	FLinearColor LightsColorOnBrake;
	
	UPROPERTY(Category=Lights, EditAnywhere)
	float LightsFlashingOnBrake;

	UPROPERTY(Category=Lights, EditAnywhere)
	FLinearColor LightsColorOnCrash;

	UPROPERTY(Category=Lights, EditAnywhere)
	float LightsFlashingOnCrash;

	float GetTurboCurrentBattery() const { return TurboCurrentBattery; }

	bool IsDriving() const { return bDrive; }
	bool IsBraking() const { return bBrake; }
	bool IsTurning() const { return bTurn; }
	bool IsUsingTurbo() const { return bTurbo; }
	
};
