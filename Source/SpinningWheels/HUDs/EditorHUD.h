// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "SpinningWheels/Core/Track.h"
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
	void InitializeOverlayBuildMenu();
	void InitializeOverlaySavePopup();
	void InitializeOverlayTrackData();
	
	void InitializeDelegates();

	void ShowModalOverlay(const TSharedPtr<SWidget>& Widget);
	void HideModalOverlay();

	UFUNCTION() void OnMenuSlotSelected(int8 Slot);
	UFUNCTION() void OnExitBuildMode();

	UFUNCTION() void OnTrackSaved(const FTrack& CurrentTrack, const bool bSuccess);

	/** Begin Pointers to overlays */
	TSharedPtr<SOverlay> RootOverlay;
	TSharedPtr<SConstraintCanvas> RootCanvas;
	TSharedPtr<SOverlay> ModalOverlay;
	
	TSharedPtr<class SEditorBuildMenuOverlay> EditorBuildMenuOverlay;
	TSharedPtr<class SEditorActionsOverlay> EditorActionsOverlay;
	TSharedPtr<class SEditorTrackDataOverlay> EditorTrackDataOverlay;
	TSharedPtr<class SSaveTrackPopup> SaveTrackPopup;
	/** End Pointers to overlays */
	
	FReply OnTestTrack();
	FReply OnSaveTrack();
	FReply OnConfirmSaveTrack(const FString& TrackName);
	FReply OnCancelSaveTrack();
	
	FReply OnMenuSelected(UEditorBuildMenuDataAsset* Menu);
	FReply OnBlockSelected(const int8 Slot);

protected:

	virtual void BeginPlay() override;

public:
	
};
