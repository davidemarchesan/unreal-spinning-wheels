// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpinningWheels/Core/Simulation.h"
#include "Car.generated.h"

class UArrowComponent;
class USkeletalMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UCarMovementComponent;
class ARacePlayerState;

UCLASS()
class SPINNINGWHEELS_API ACar : public APawn
{
	GENERATED_BODY()

public:
	
	ACar(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

private:

	// Begin Deterministic physics
	TWeakObjectPtr<ARacePlayerState> RacePlayerState;
	
	void SimulatedTick(float DeltaTime);
	
	float AccSimulationTime = 0.f;
	float LastSimulationDelta = 0.f;
	float TotSeconds = 0.f;
	
	uint32 CurrentFrameIndex = 0;
	FSimulationFrame CurrentSimulationFrame;
	FSimulationFrame PreviousSimulationFrame;

	void SetSimulationFrame(FSimulationFrame NewSimulationFrame);
	// End Deterministic physics

	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(Category=Car, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(Category=Car, VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCarMovementComponent> CarMovementComponent;
	
	UPROPERTY(ReplicatedUsing=OnRep_BrakeUpdate) bool bBrake = false;
	UFUNCTION() void OnRep_BrakeUpdate();
	
	UPROPERTY(ReplicatedUsing=OnRep_TurboUpdate) bool bTurbo = false;
	UFUNCTION() void OnRep_TurboUpdate();
	
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
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	void SetPlayerState(ARacePlayerState* NewRacePlayerState);

	UCarMovementComponent* GetCarMovementComponent() const { return CarMovementComponent; }
	
	void LocalBrakeLights();
	void LocalTurboLights();

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
	
};
