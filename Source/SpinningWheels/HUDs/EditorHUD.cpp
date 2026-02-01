// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorHUD.h"

#include "SpinningWheels/Controllers/EditorController.h"
#include "UI/Slate/Overlays/EditorBuildMenu/EditorBuildMenuOverlay.h"

void AEditorHUD::InitializeOverlays()
{
}

void AEditorHUD::InitializeDelegates()
{
	if (GEngine == nullptr)
	{
		return;
	}
}

void AEditorHUD::OnMenuSlotSelected(int8 Slot)
{
	UE_LOG(LogTemp, Warning, TEXT("HUD selected slot %d"), Slot);
	if (EditorBuildMenuOverlay.IsValid())
	{
		EditorBuildMenuOverlay->OnMenuSlotSelected(Slot);
	}
}

void AEditorHUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeOverlays();
	InitializeDelegates();
}

void AEditorHUD::InitializeBuildMenu(AEditorController* Controller, const FEditorBuildMenu& CurrentActiveMenu)
{
	if (GEngine == nullptr)
	{
		return;
	}

	EditorBuildMenuOverlay = SNew(SEditorBuildMenuOverlay)
		.Menu(CurrentActiveMenu);
	if (EditorBuildMenuOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(EditorBuildMenuOverlay.ToSharedRef());
	}

	if (Controller)
	{
		Controller->OnMenuSlotSelected.AddDynamic(this, &AEditorHUD::OnMenuSlotSelected);
	}
}
