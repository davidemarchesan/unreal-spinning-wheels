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

protected:
	virtual void BeginPlay() override;

public:
	
	UCarMovementComponent();

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadOnly, Transient, DuplicateTransient, Category=MovementComponent)
	TObjectPtr<USceneComponent> UpdatedComponent;

	UPROPERTY(EditAnywhere, Category=Velocity)
	FVector Velocity;

	void AddInputVector(FVector WorldVector);

	static const float MIN_TICK_TIME;

		
};
