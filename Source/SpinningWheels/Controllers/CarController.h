// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "CarController.generated.h"

class UInputMappingContext;
class UDriveInputConfig;
class UEditorInputConfig;
class ACar;
class AEditorPawn;
class AMainCamera;

/**
 * Debug use only
 */
UCLASS()
class SPINNINGWHEELS_API ACarController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> DriveMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDriveInputConfig> DriveInputConfig;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> EditorMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorInputConfig> EditorInputConfig;

	UPROPERTY(Category=Classes, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<ACar> CarClass;
	
	UPROPERTY(Category=Classes, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AEditorPawn> EditorClass;
	
	UPROPERTY(Category=Classes, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AMainCamera> CameraClass;

	TWeakObjectPtr<ACar> Car;
	TWeakObjectPtr<AEditorPawn> Editor;
	TWeakObjectPtr<AMainCamera> MainCamera;

	void SetupCamera();

	/** Input Actions handler - Drive */
	void StartDrive();
	void StopDrive();
	void StartBrake();
	void StopBrake();
	void Turn(const FInputActionValue& Value);
	void StartTurbo();
	void StopTurbo();
	void SwitchToEditor();
	
	/** Input Actions handler - Editor */
	void MoveCamera(const FInputActionValue& Value);
	void RotateCamera(const FInputActionValue& Value);
	void SwitchToDrive();

protected:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

public:
	
	virtual void SetPawn(APawn* InPawn) override;
	
};
