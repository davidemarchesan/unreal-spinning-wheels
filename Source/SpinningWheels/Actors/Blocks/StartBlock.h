// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include "StartBlock.generated.h"

class USceneComponent;
class UArrowComponent;

UCLASS()
class SPINNINGWHEELS_API AStartBlock : public ABlock
{
	GENERATED_BODY()

public:

	AStartBlock();

private:

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> StartSceneComponent;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UArrowComponent> ArrowComponent;

protected:

public:

	void GetStartingPoint(FVector& OutLocation, FRotator& OutDirection);
	
};
