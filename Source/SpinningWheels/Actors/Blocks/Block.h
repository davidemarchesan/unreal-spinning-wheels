#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

class UMeshComponent;

USTRUCT()
struct FBlockRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlock> BlockClass;

	UPROPERTY(EditAnywhere)
	FText Name;
	
};

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
