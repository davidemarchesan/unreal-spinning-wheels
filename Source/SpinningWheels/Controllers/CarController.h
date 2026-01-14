// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "CarController.generated.h"

class UInputMappingContext;
class UDriveInputConfig;

UCLASS()
class SPINNINGWHEELS_API ACarController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDriveInputConfig> InputConfig;

protected:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:

	/** Actions */

	void Drive();
	void Brake();
	void Turn();
	
	
};
