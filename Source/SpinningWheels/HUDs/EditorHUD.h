// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"
#include "EditorHUD.generated.h"

class AEditorController;

UCLASS()
class SPINNINGWHEELS_API AEditorHUD : public AHUD
{
	GENERATED_BODY()

private:

	void InitializeOverlays();
	void InitializeDelegates();

	UFUNCTION() void OnMenuSlotSelected(int8 Slot);

	/** Begin Pointers to overlays */
	TSharedPtr<class SEditorBuildMenuOverlay> EditorBuildMenuOverlay;
	/** End Pointers to overlays */

protected:

	virtual void BeginPlay() override;

public:

	void InitializeBuildMenu(AEditorController* Controller, const FEditorBuildMenu& CurrentActiveMenu);
	
};
