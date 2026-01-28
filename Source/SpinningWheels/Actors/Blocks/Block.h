#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

class UMeshComponent;

UCLASS()
class SPINNINGWHEELS_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlock();

private:

	UPROPERTY(VisibleDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMeshComponent> MeshComponent;

protected:

public:
	
};
