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

void UCarMovementComponent::AddControlInput(FVector InputVector)
{
	ControlInputVector += InputVector;
}

void UCarMovementComponent::ConsumeControlInput()
{
	LastControlInputVector = ControlInputVector;
	ControlInputVector = FVector::ZeroVector;
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

void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	// Input from player
	Velocity += ControlInputVector;

	if (Velocity.IsNearlyZero() == false)
	{
		FVector SimpleDrag = FVector(-0.3f, 0.f, 0.f);
		Velocity += SimpleDrag;
	}


	// Velocity = Velocity * DeltaTime;
	// Velocity = FVector(2000.f, 0.f, 0.f) * DeltaTime;
	// UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *Velocity.ToString());
	ConsumeControlInput();

	if (UpdatedComponent)
	{
		UpdatedComponent->MoveComponent(Velocity * DeltaTime, UpdatedComponent->GetComponentRotation(), true);
	}
}

void UCarMovementComponent::Drive()
{
	if (AccelerationCurve == nullptr)
	{
		const float Alpha = Velocity.IsNearlyZero() ? 0.f : Velocity.Length() / MaxSpeed;
		CurrentAcceleration = FMath::InterpEaseIn(MaxAcceleration, 0.f, Alpha, 2.f);
		UE_LOG(LogTemp, Warning, TEXT("MaxAcceleration: %f"), MaxAcceleration);
		UE_LOG(LogTemp, Warning, TEXT("CurrentAcceleration: %f at Alpha %f"), CurrentAcceleration, Alpha);

		AddControlInput(FVector(1.f * CurrentAcceleration, 0.f, 0.f));
	}
	else
	{
		const float Alpha = Velocity.IsNearlyZero() ? 0.f : Velocity.Length() / MaxSpeed;
		const float CurveMultiplier = AccelerationCurve->GetFloatValue(Alpha);
		CurrentAcceleration = MaxAcceleration * CurveMultiplier;
		UE_LOG(LogTemp, Warning, TEXT("MaxAcceleration: %f"), MaxAcceleration);
		UE_LOG(LogTemp, Warning, TEXT("CurveMultiplier: %f"), CurveMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("CurrentAcceleration: %f"), CurrentAcceleration);

		AddControlInput(FVector(1.f * CurrentAcceleration, 0.f, 0.f));
	}
}
