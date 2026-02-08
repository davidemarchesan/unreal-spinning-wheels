// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "SpinningWheels/Core/Track.h"
#include "SpinningWheels/GameModes/EditorGameMode.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "EditorHUD.generated.h"

class AEditorGameMode;
class AEditorController;

UCLASS()
class SPINNINGWHEELS_API AEditorHUD : public AHUD
{
	GENERATED_BODY()

private:

	TWeakObjectPtr<AEditorGameMode> EditorGameMode;
	TWeakObjectPtr<AEditorController> EditorController;
	
	void InitializeRootOverlay();
	void InitializeOverlayEditorActions();
	void InitializeOverlayEditorBuildMenu();
	void InitializeOverlayEditorSavePopup();
	void InitializeOverlayEditorMenu();
	void InitializeOverlayEditorTrackData();

	void ShowModalOverlay(const TSharedPtr<SWidget>& Widget, const bool bFocus = true);
	void HideModalOverlay();

	/** Begin delegates bindings */
	void InitializeDelegates();
	void DeinitializeDelegates();
	
	UFUNCTION() void OnMenuSlotSelected(int8 Slot);
	UFUNCTION() void OnExitBuildMode();

	UFUNCTION() void OnTrackLoaded(const FTrack& CurrentTrack);
	UFUNCTION() void OnTrackSaved(const FTrack& CurrentTrack, const bool bSuccess);
	UFUNCTION() void OnEditorModeChanged(const EEditorMode EditorMode);
	/** End delegates bindings */

	/** Begin Pointers to overlays */
	TSharedPtr<SOverlay> RootOverlay;
	TSharedPtr<SConstraintCanvas> RootCanvas;
	TSharedPtr<SOverlay> ModalOverlay;
	
	TSharedPtr<class SEditorBuildMenuOverlay> EditorBuildMenuOverlay;
	TSharedPtr<class SEditorActionsOverlay> EditorActionsOverlay;
	TSharedPtr<class SEditorTrackDataOverlay> EditorTrackDataOverlay;
	TSharedPtr<class SEditorSaveTrackPopup> SaveTrackPopup;
	TSharedPtr<class SEditorMenuPopup> EditorMenuPopup;
	/** End Pointers to overlays */
	
	FReply OnTestTrack();
	FReply OnSaveTrack();
	FReply OnConfirmSaveTrack(const FString& TrackName);
	
	FReply OnMenuSelected(UEditorBuildMenuDataAsset* Menu);
	FReply OnBlockSelected(const int8 Slot);

	FReply OnReturnToEditor();
	FReply OnGoToMainMenu();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	void InputOpenMenu();	
	
};
