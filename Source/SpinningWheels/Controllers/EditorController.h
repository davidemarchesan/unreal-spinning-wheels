// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceController.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "EditorController.generated.h"

class UInputMappingContext;
class UEditorInputConfig;
class UBuildInputConfig;
class UEditorBuildMenu;
class AEditorPawn;
class AEditorHUD;
class ATrackGrid;
class ABlock;

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

	void InputBuildBlock();
	void InputBuildCancel();
	void InputBuildRotateBlock(const FInputActionValue& Value);

	void PreviewBlock();

	TWeakObjectPtr<AEditorHUD> HUD;
	TWeakObjectPtr<ATrackGrid> TrackGrid;

	TSubclassOf<ABlock> BlockClass;

	UFUNCTION()
	void OnTrackGridReady(ATrackGrid* InTrackGrid);
	
protected:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> EditorMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorInputConfig> EditorInputConfig;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> EditorBuildMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBuildInputConfig> EditorBuildInputConfig;

	// UPROPERTY(Category=Data, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	// TObjectPtr<UDataTable> BlocksTableData;
	
	UPROPERTY(Category=Data, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorBuildMenuDataAsset> EditorBuildMenuData;

	void SetupBuildMenu();
	FEditorBuildMenu CurrentActiveMenu;

	bool bBuildMode = false;
	
	void EnterBuildMode(TSubclassOf<ABlock> NewBlockClass);
	void ExitBuildMode();

	//~ Begin AController Interface
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;
	//~ End AController Interface

	virtual void SetupInputBindings() override;
	void SetupEditorInputBindings();
	void SetupBuildInputBindings();

	virtual void EnableDefaultInputMappingContext() override;
	void EnableEditorInputMappingContext();
	void DisableEditorInputMappingContext();
	void EnableBuildInputMappingContext();
	void DisableBuildInputMappingContext();

public:

	FOnMenuSlotSelectedSignature OnMenuSlotSelected;

	void SetTrackGrid(const TWeakObjectPtr<ATrackGrid> InTrackGrid) { TrackGrid = InTrackGrid;};
	
	void InputSlot(int8 Slot);
	
};
