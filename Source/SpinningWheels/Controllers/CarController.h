// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "CarController.generated.h"

class UInputMappingContext;
class UDriveInputConfig;
class ACar;

/**
 * Debug use only
 */
UCLASS()
class SPINNINGWHEELS_API ACarController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDriveInputConfig> InputConfig;

	ACar* Car;

	/** Input Actions handler */
	void Drive();
	void Brake();
	void Turn();

protected:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

public:
	
	virtual void SetPawn(APawn* InPawn) override;
	
};
