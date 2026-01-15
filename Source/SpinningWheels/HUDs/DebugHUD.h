// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DebugHUD.generated.h"

class ACar;

/**
 * Debug use only
 */
UCLASS()
class SPINNINGWHEELS_API ADebugHUD : public AHUD
{
	GENERATED_BODY()

private:

	TWeakObjectPtr<ACar> Car;

protected:

	virtual void BeginPlay() override;

public:
	
};
