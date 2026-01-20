// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DriveInputConfig.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API UDriveInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Drive; // Accelerate
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Brake;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Turn;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Turbo;
	
};
