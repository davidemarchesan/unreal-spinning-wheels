// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceController.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "EditorController.generated.h"

class UInputMappingContext;
class UEditorInputConfig;
class UEditorBuildMenu;
class AEditorPawn;
class AEditorHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuSlotSelectedSignature, int8, Slot);

UCLASS()
class SPINNINGWHEELS_API AEditorController : public ARaceController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

private:

	TWeakObjectPtr<AEditorPawn> EditorPawn;

	void InputMoveCamera(const FInputActionValue& Value);
	void InputRotateCamera(const FInputActionValue& Value);

	void InputSlot1();
	void InputSlot2();
	void InputSlot3();
	void InputSlot4();
	void InputSlot5();
	void InputSlot6();
	void InputSlot7();
	void InputSlot8();
	void InputSlot9();

	TWeakObjectPtr<AEditorHUD> HUD;
	
protected:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> EditorMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorInputConfig> EditorInputConfig;

	UPROPERTY(Category=EditorBuildMenu, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorBuildMenuDataAsset> EditorBuildMenuData;

	FEditorBuildMenu CurrentActiveMenu;

	//~ Begin AController Interface
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;
	//~ End AController Interface

	virtual void SetupInputBindings() override;
	void SetupEditorInputBindings();

public:

	FOnMenuSlotSelectedSignature OnMenuSlotSelected;

	void InputSlot(int8 Slot);
	
};
