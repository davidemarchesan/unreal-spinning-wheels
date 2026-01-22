#include "Block.h"

ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	if (USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene"))
	{
		RootComponent = SceneComponent;
	}

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
	}

}

