// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorHUD.h"

#include "SpinningWheels/Controllers/EditorController.h"
#include "UI/Slate/Overlays/Editor/EditorOverlay.h"
#include "UI/Slate/Overlays/EditorBuildMenu/EditorBuildMenuOverlay.h"

void AEditorHUD::InitializeOverlays()
{
	if (GEngine == nullptr)
	{
		return;
	}

	EditorOverlay = SNew(SEditorOverlay)
	.OnSaveTrack_Lambda([this](const FString& TrackName)
	{
		if (this)
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD lambda EditorOverlay OnSaveTrack_Lambda %s"), *TrackName);
			return OnSaveTrack(TrackName);
		}

		return FReply::Unhandled();
	});
	if (EditorOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(EditorOverlay.ToSharedRef());
	}
}

void AEditorHUD::InitializeDelegates()
{
}

void AEditorHUD::OnMenuSlotSelected(int8 Slot)
{
	UE_LOG(LogTemp, Warning, TEXT("HUD selected slot %d"), Slot);
	if (EditorBuildMenuOverlay.IsValid())
	{
		EditorBuildMenuOverlay->OnMenuSlotSelected(Slot);
	}
}

FReply AEditorHUD::OnSaveTrack(const FString& TrackName)
{
	UE_LOG(LogTemp, Warning, TEXT("HUD function EditorOverlay OnSaveTrack_Lambda %s"), *TrackName);

	if (AEditorController* EditorController = Cast<AEditorController>(GetOwningPlayerController()))
	{
		EditorController->InputSaveTrack(TrackName);
	}
	
	return FReply::Handled();
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
