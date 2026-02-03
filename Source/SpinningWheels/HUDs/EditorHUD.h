// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "UI/Slate/Overlays/Editor/EditorOverlay.h"
#include "EditorHUD.generated.h"

class AEditorController;

UCLASS()
class SPINNINGWHEELS_API AEditorHUD : public AHUD
{
	GENERATED_BODY()

private:

	TWeakObjectPtr<AEditorController> EditorController;
	
	void InitializeOverlays();
	void InitializeDelegates();

	UFUNCTION() void OnMenuSlotSelected(int8 Slot);
	UFUNCTION() void OnExitBuildMode();

	/** Begin Pointers to overlays */
	TSharedPtr<class SEditorBuildMenuOverlay> EditorBuildMenuOverlay;
	TSharedPtr<class SEditorOverlay> EditorOverlay;
	/** End Pointers to overlays */
	
	FReply OnSaveTrack(const FString& TrackName);
	FReply OnMenuSelected(UEditorBuildMenuDataAsset* Menu);
	FReply OnBlockSelected(const int8 Slot);

protected:

	virtual void BeginPlay() override;

public:

	void InitializeBuildMenu(AEditorController* Controller, const FEditorBuildMenu& CurrentActiveMenu);
	
};
