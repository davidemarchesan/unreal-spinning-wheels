#include "Car.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CarMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SpinningWheels/Actors/Blocks/CheckpointBlock.h"
#include "SpinningWheels/Actors/Blocks/FinishBlock.h"
#include "SpinningWheels/PlayerStates/RacePlayerState.h"

ACar::ACar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	if (BoxComponent)
	{
		BoxComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		BoxComponent->SetShouldUpdatePhysicsVolume(true);
		BoxComponent->SetCanEverAffectNavigation(false);
		BoxComponent->SetBoxExtent(FVector(183.f, 97.f, 48.f));
		BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
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

	// Camera inside pawn deprecated
	// SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Camera Arm");
	// if (SpringArmComponent)
	// {
	// 	SpringArmComponent->SetupAttachment(RootComponent);
	// 	SpringArmComponent->SetRelativeLocation(FVector(-250.f, 0.f, 100.f));
	//
	// 	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	// 	if (CameraComponent)
	// 	{
	// 		CameraComponent->SetupAttachment(SpringArmComponent);
	// 	}
	// }

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

void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ConsumeTurbo(DeltaTime);
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACar, bBrake, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ACar, bTurbo, COND_SkipOwner);
}

void ACar::OnRep_BrakeUpdate()
{
	if (bBrake == true)
	{
		LocalBrakeLights();
	}
	else
	{
		StopLights();
	}
}

void ACar::OnRep_TurboUpdate()
{
	if (bTurbo == true)
	{
		LocalTurboLights();
	}
	else
	{
		StopLights();
	}
}

void ACar::ConsumeTurbo(float DeltaTime)
{
	if (bTurbo == true)
	{
		TurboCurrentBattery = FMath::Clamp(TurboCurrentBattery - (TurboConsumption * DeltaTime), 0.f, TurboMaxBattery);
		if (TurboCurrentBattery <= 0.f)
		{
			InputStopTurbo();
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

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACar::OnComponentBeginOverlap);
	}

	if (ARacePlayerState* PS = Cast<ARacePlayerState>(GetPlayerState()))
	{
		PS->OnStartLap();
	}
}

void ACar::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (IsLocallyControlled() || HasAuthority())
	{
		// Controller should always be true, but you never know
		if (AController* PC = GetController())
		{
			if (PC->PlayerState)
			{
				if (OtherActor->IsA(ACheckpointBlock::StaticClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("ACar Checkpoint!"));
					if (ARacePlayerState* PS = Cast<ARacePlayerState>(GetPlayerState()))
					{
						PS->OnCheckpoint();
					}
				}
				if (OtherActor->IsA(AFinishBlock::StaticClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("ACar Finish line!"));
					if (ARacePlayerState* PS = Cast<ARacePlayerState>(GetPlayerState()))
					{
						PS->OnFinishLap();
					}
				}
			}
		}
	}


	// // Server
	// if (HasAuthority())
	// {
	//
	// 	
	// 	
	// }
	//
	// // if (HasAuthority() == false)
	// // {
	// // 	return;
	// // }
	//
	// // prediction
	// // server
	//
	// if (OtherActor->IsA(ACheckpointBlock::StaticClass()))
	// {
	// 	if (AController* PC = GetController())
	// 	{
	// 		if (PC->PlayerState != nullptr)
	// 		{
	// 			ENetRole LR = GetLocalRole();
	// 			int32 PID = GetController()->PlayerState->GetPlayerId();
	// 			if (ARacePlayerState* PS = Cast<ARacePlayerState>(GetPlayerState()))
	// 			{
	// 				PS->OnCheckpoint();
	// 			}
	// 			UE_LOG(LogTemp, Warning, TEXT("Car: i have passed a checkpoint! id %d with role %d"), PID, LR);
	// 		} else { UE_LOG(LogTemp, Error, TEXT("no ps")); }
	// 	} else { UE_LOG(LogTemp, Error, TEXT("no pc")); }
	// }
}

void ACar::LocalStartDrive()
{
	bDrive = true;

	if (CarMovementComponent)
	{
		CarMovementComponent->StartDrive();
	}
}

void ACar::InputStartDrive()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		LocalStartDrive();
		ServerStartDrive();
	}

	if (HasAuthority() && IsLocallyControlled())
	{
		LocalStartDrive();
	}
}

void ACar::ServerStartDrive_Implementation()
{
	LocalStartDrive();
}

void ACar::LocalStopDrive()
{
	bDrive = false;

	if (CarMovementComponent)
	{
		CarMovementComponent->StopDrive();
	}
}

void ACar::InputStopDrive()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		LocalStopDrive();
		ServerStopDrive();
	}

	if (HasAuthority() && IsLocallyControlled())
	{
		LocalStopDrive();
	}
}

void ACar::ServerStopDrive_Implementation()
{
	LocalStopDrive();
}

void ACar::LocalStartBrake()
{
	InputStopTurbo();
	bBrake = true;

	if (CarMovementComponent)
	{
		CarMovementComponent->StartBrake();
	}
}

void ACar::InputStartBrake()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		LocalStartBrake();
		ServerStartBrake();

		OnRep_BrakeUpdate();
	}

	if (HasAuthority() && IsLocallyControlled())
	{
		LocalStartBrake();
		OnRep_BrakeUpdate();
	}
}

void ACar::ServerStartBrake_Implementation()
{
	LocalStartBrake();
	OnRep_BrakeUpdate();
}

void ACar::LocalBrakeLights()
{
	UpdateLightsBehavior(1.0f, LightsColorOnBrake, LightsFlashingOnBrake);
}

void ACar::LocalStopBrake()
{
	bBrake = false;

	if (CarMovementComponent)
	{
		CarMovementComponent->StopBrake();
	}
}

void ACar::InputStopBrake()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		LocalStopBrake();
		ServerStopBrake();

		OnRep_BrakeUpdate();
	}

	if (HasAuthority() && IsLocallyControlled())
	{
		LocalStopBrake();
		OnRep_BrakeUpdate();
	}
}

void ACar::ServerStopBrake_Implementation()
{
	LocalStopBrake();
	OnRep_BrakeUpdate();
}

void ACar::LocalTurn(FVector2D InputVector)
{
	if (CarMovementComponent)
	{
		CarMovementComponent->Turn(InputVector);
	}
}

void ACar::InputTurn(FVector2D InputVector)
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		LocalTurn(InputVector);
		ServerTurn(InputVector);
	}

	if (HasAuthority() && IsLocallyControlled())
	{
		LocalTurn(InputVector);
	}
}

void ACar::ServerTurn_Implementation(FVector2D InputVector)
{
	LocalTurn(InputVector);
}

void ACar::LocalStartTurbo()
{
	if (TurboCurrentBattery > 0.f)
	{
		bTurbo = true;

		if (CarMovementComponent)
		{
			CarMovementComponent->StartTurbo();
		}
	}
}

void ACar::InputStartTurbo()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		LocalStartTurbo();
		ServerStartTurbo();

		OnRep_TurboUpdate();
	}

	if (HasAuthority() && IsLocallyControlled())
	{
		LocalStartTurbo();
		OnRep_TurboUpdate();
	}
}

void ACar::ServerStartTurbo_Implementation()
{
	LocalStartTurbo();
	OnRep_TurboUpdate();
}

void ACar::LocalTurboLights()
{
	UpdateLightsBehavior(1.0f, LightsColorOnTurbo, LightsFlashingOnTurbo);
}

void ACar::LocalStopTurbo()
{
	bTurbo = false;

	if (CarMovementComponent)
	{
		CarMovementComponent->StopTurbo();
	}
}

void ACar::InputStopTurbo()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		LocalStopTurbo();
		ServerStopTurbo();

		OnRep_TurboUpdate();
	}

	if (HasAuthority() && IsLocallyControlled())
	{
		LocalStopTurbo();
		OnRep_TurboUpdate();
	}
}

void ACar::ServerStopTurbo_Implementation()
{
	LocalStopTurbo();
	OnRep_TurboUpdate();
}

void ACar::ToggleTurbo()
{
}
