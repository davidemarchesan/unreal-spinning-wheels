#include "Block.h"

ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	if (USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene"))
	{
		RootComponent = SceneComponent;
	}

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
		MeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}

void ABlock::ShowOutline()
{
	if (MeshComponent)
	{
		MeshComponent->SetCustomDepthStencilValue(1);
		MeshComponent->SetRenderCustomDepth(true);
	}
}

void ABlock::HideOutline()
{
	if (MeshComponent)
	{
		MeshComponent->SetRenderCustomDepth(false);
	}
}

FName ABlock::GetBlockId() const
{
	if (BlocksTableRow.IsNull() == false)
	{
		return BlocksTableRow.RowName;
	}

	return ""; // todo: fname::empty?
}
