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

	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_SelectBlock;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_RemoveBlock;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot1;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot2;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot3;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot4;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot5;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot6;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot7;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot8;
	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_Slot9;
	
	// UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_OpenMenu;
};
