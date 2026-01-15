#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CarMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPINNINGWHEELS_API UCarMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	void SetUpdatedComponent(USceneComponent* InUpdatedComponent) { UpdatedComponent = InUpdatedComponent; }

	FVector Velocity = FVector::ZeroVector;

	float Grip = 1.f;
	
	FVector ControlInputVector = FVector::ZeroVector;
	FVector LastControlInputVector = FVector::ZeroVector;

	void AddControlInput(FVector InputVector);
	void ConsumeControlInput();
	
	float CurrentSpeed = 0.f;
	float CurrentAcceleration = 0.f;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(Category=Engine, EditAnywhere, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s2"))
	float MaxAcceleration;

	UPROPERTY(Category=Engine, EditAnywhere)
	TObjectPtr<UCurveFloat> AccelerationCurve;

	UPROPERTY(Category=Engine, EditAnywhere, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MaxSpeed;

public:

	static const float MIN_TICK_TIME;
	
	UCarMovementComponent();

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadOnly, Transient, DuplicateTransient, Category=MovementComponent)
	TObjectPtr<USceneComponent> UpdatedComponent;

	void Drive(); // Rear wheels pushing

	FVector GetVelocity() const { return Velocity; }
	
	float GetCurrentSpeed() const { return CurrentSpeed; }
	float GetCurrentAcceleration() const { return CurrentAcceleration; }
		
};
