// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EditorInputConfig.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API UEditorInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Rotate;

	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_F1;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_F2;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_F3;
};
