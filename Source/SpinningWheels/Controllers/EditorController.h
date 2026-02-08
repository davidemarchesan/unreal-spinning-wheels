// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceController.h"
#include "SpinningWheels/Actors/Blocks/Block.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "SpinningWheels/GameModes/EditorGameMode.h"
#include "EditorController.generated.h"

class UInputMappingContext;
class UEditorInputConfig;
class UBuildInputConfig;
class UEditorBuildMenu;
class AEditorPawn;
class AEditorGameMode;
class AEditorHUD;
class ATrackGrid;
class ABlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuSlotSelectedSignature, int8, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitBuildModeSignature);

UCLASS()
class SPINNINGWHEELS_API AEditorController : public ARaceController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

private:

	EEditorMode EditorMode = EEditorMode::EM_Editor;;

	bool bIgnoreInput = false;
	
	void InputMoveCamera(const FInputActionValue& Value);
	void InputRotateCamera(const FInputActionValue& Value);

	void InputSelectBlock();
	void InputRemoveBlock();

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

	TWeakObjectPtr<AEditorPawn> EditorPawn;
	TWeakObjectPtr<AEditorGameMode> EditorGameMode;
	TWeakObjectPtr<AEditorHUD> EditorHUD;
	TWeakObjectPtr<ATrackGrid> TrackGrid;

	FName BlockToBuildName;

	void PreviewBlock();
	TWeakObjectPtr<ABlock> PreviewedBlock;

	void HoverBlock();
	TWeakObjectPtr<ABlock> HoveredBlock;

	void InitializeDelegates();
	void DeinitializeDelegates();

	UFUNCTION() void OnTrackGridReady(ATrackGrid* InTrackGrid);
	UFUNCTION() void OnEditorModeChanged(const EEditorMode NewEditorMode);

	void MovePawnAtCenter();
	
protected:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> EditorMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorInputConfig> EditorInputConfig;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> EditorBuildMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBuildInputConfig> EditorBuildInputConfig;
	
	UPROPERTY(Category=Data, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorBuildMenuDataAsset> EditorBuildMenuData;

	void SetupBuildMenu();
	FEditorBuildMenu CurrentActiveMenu;

	bool bBuildMode = false;
	bool bMovingBlock = false;
	
	void EnterBuildMode(const FName& RowName, const FBlockRow& BlockRow);
	void EnterBuildModeWithHovered();
	void ExitBuildMode();

	//~ Begin AController Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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

	virtual void InputOpenMenu() override;
	
	void SetupTrackGrid();

public:

	void BlockCursor();
	void UnlockCursor();

	FOnMenuSlotSelectedSignature OnMenuSlotSelected;
	FOnExitBuildModeSignature OnExitBuildMode;

	FEditorBuildMenu GetCurrentActiveMenu() const { return CurrentActiveMenu; };

	void SetTrackGrid(const TWeakObjectPtr<ATrackGrid> InTrackGrid) { TrackGrid = InTrackGrid;};
	
	void InputSlot(int8 Slot);
	void InputSelectMenu(UEditorBuildMenuDataAsset* Menu);

	void InputTestTrack();
	void InputReturnToEditor();
	void InputSaveTrack(const FString& TrackName);

	FTrack GetCurrentTrack();
	FString GetTrackName();
};
