#include "CarMovementComponent.h"
#include "Components/BoxComponent.h"

const float UCarMovementComponent::MIN_TICK_TIME = 1e-6f;

UCarMovementComponent::UCarMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCarMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	UE_LOG(LogTemp, Warning, TEXT("INITIALIZE COMPONENT"));

	// if (AActor* Owner = GetOwner())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("getting owner"));
	// 	if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(Owner->GetRootComponent()))
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("is a box"));
	// 		SetUpdatedComponent(BoxComponent);
	// 	}
	// }
}

void UCarMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("getting owner"));
		if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(Owner->GetRootComponent()))
		{
			UE_LOG(LogTemp, Warning, TEXT("is a box"));
			SetUpdatedComponent(BoxComponent);
		}
	}
}

void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (UpdatedComponent)
	{
		UpdatedComponent->MoveComponent(FVector(4.f, 0.f, 0.f), UpdatedComponent->GetComponentRotation(), true);
	}
}

void UCarMovementComponent::AddInputVector(FVector WorldVector)
{
}

