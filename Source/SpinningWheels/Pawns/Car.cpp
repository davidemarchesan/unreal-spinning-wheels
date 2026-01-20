#include "Car.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CarMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACar::ACar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	if (BoxComponent)
	{
		BoxComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		BoxComponent->SetShouldUpdatePhysicsVolume(true);
		BoxComponent->SetCanEverAffectNavigation(false);
		BoxComponent->SetBoxExtent(FVector(183.f, 97.f, 48.f));
		RootComponent = BoxComponent;
	}

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->AlwaysLoadOnClient = true;
		SkeletalMeshComponent->AlwaysLoadOnServer = true;
		SkeletalMeshComponent->bOwnerNoSee = false;
		SkeletalMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		SkeletalMeshComponent->bCastDynamicShadow = true;
		SkeletalMeshComponent->bAffectDynamicIndirectLighting = true;
		SkeletalMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		SkeletalMeshComponent->SetGenerateOverlapEvents(false);
		SkeletalMeshComponent->SetCanEverAffectNavigation(false);
		SkeletalMeshComponent->SetRelativeLocation(FVector(-2.6f, 0.f, -48.f));
		SkeletalMeshComponent->SetupAttachment(RootComponent);
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Camera Arm");
	if (SpringArmComponent)
	{
		SpringArmComponent->SetupAttachment(RootComponent);
		SpringArmComponent->SetRelativeLocation(FVector(-250.f, 0.f, 100.f));

		CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
		if (CameraComponent)
		{
			CameraComponent->SetupAttachment(SpringArmComponent);
		}
	}

	CarMovementComponent = CreateDefaultSubobject<UCarMovementComponent>("Movement");
	if (CarMovementComponent)
	{
		CarMovementComponent->UpdatedComponent = RootComponent;
	}

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>("Arrow");
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SpriteInfo.Category = "Cars";
		ArrowComponent->SpriteInfo.DisplayName = NSLOCTEXT("SpriteCategory", "Cars", "Cars");
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
		ArrowComponent->SetupAttachment(RootComponent);
	}
#endif
}

void ACar::ConsumeTurbo(float DeltaTime)
{
	if (bTurbo == true)
	{
		TurboCurrentBattery = FMath::Clamp(TurboCurrentBattery - (TurboConsumption * DeltaTime), 0.f, TurboMaxBattery);
		if (TurboCurrentBattery <= 0.f)
		{
			StopTurbo();
		}
	}
}

void ACar::UpdateLightsBehavior(float InIntensity, FLinearColor InColor, float InFlashing)
{
	if (DynamicMaterialLights.IsValid())
	{
		DynamicMaterialLights->SetVectorParameterValue("LightColor", InColor);
		DynamicMaterialLights->SetScalarParameterValue("Flashing", InFlashing);
		DynamicMaterialLights->SetScalarParameterValue("Intensity", InIntensity);
	}
}

void ACar::StopLights()
{
	if (DynamicMaterialLights.IsValid())
	{
		DynamicMaterialLights->SetScalarParameterValue("Intensity", 0.f);
	}
}

void ACar::BeginPlay()
{
	Super::BeginPlay();

	if (SkeletalMeshComponent)
	{
		DynamicMaterialLights = SkeletalMeshComponent->CreateDynamicMaterialInstance(ACar::MaterialIndexLights);
	}
}

void ACar::StartDrive()
{
	bDrive = true;
	
	if (CarMovementComponent)
	{
		CarMovementComponent->StartDrive();
	}
}

void ACar::StopDrive()
{
	bDrive = false;
	
	if (CarMovementComponent)
	{
		CarMovementComponent->StopDrive();
	}
}

void ACar::StartBrake()
{
	StopTurbo();
	UpdateLightsBehavior(1.0f, LightsColorOnBrake, LightsFlashingOnBrake);
	bBrake = true;

	if (CarMovementComponent)
	{
		CarMovementComponent->StartBrake();
	}
}

void ACar::StopBrake()
{
	StopLights();
	bBrake = false;

	if (CarMovementComponent)
	{
		CarMovementComponent->StopBrake();
	}
}

void ACar::Turn(FVector2D InputVector)
{
	if (CarMovementComponent)
	{
		CarMovementComponent->Turn(InputVector);
	}
}

void ACar::StartTurbo()
{
	if (TurboCurrentBattery > 0.f)
	{
		UpdateLightsBehavior(1.0f, LightsColorOnTurbo, LightsFlashingOnTurbo);
		bTurbo = true;

		if (CarMovementComponent)
		{
			CarMovementComponent->StartTurbo();
		}
	}
}

void ACar::StopTurbo()
{
	StopLights();
	bTurbo = false;

	if (CarMovementComponent)
	{
		CarMovementComponent->StopTurbo();
	}
}

void ACar::ToggleTurbo()
{
}

void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ConsumeTurbo(DeltaTime);
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
