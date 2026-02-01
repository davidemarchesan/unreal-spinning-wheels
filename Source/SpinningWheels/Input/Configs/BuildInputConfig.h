// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildInputConfig.generated.h"

class UInputAction;

UCLASS()
class SPINNINGWHEELS_API UBuildInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Build;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Cancel;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Rotate;
	
};
