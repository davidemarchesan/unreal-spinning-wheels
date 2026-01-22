// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include "FinishBlock.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API AFinishBlock : public ABlock
{
	GENERATED_BODY()

public:

	AFinishBlock();

private:
	
	UPROPERTY(VisibleDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	
};
