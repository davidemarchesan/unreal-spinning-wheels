// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include "CheckpointBlock.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API ACheckpointBlock : public ABlock
{
	GENERATED_BODY()

public:

	ACheckpointBlock();

private:

	UPROPERTY(VisibleDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> BoxComponent;

protected:

public:

	virtual void EnableCheckpoint();
	virtual void DisableCheckpoint();
	
};
