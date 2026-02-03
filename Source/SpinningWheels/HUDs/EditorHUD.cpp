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
	if (EditorBuildMenuOverlay.IsValid())
	{
		EditorBuildMenuOverlay->OnMenuSlotSelected(Slot);
	}
}

void AEditorHUD::OnExitBuildMode()
{
	if (EditorBuildMenuOverlay.IsValid())
	{
		EditorBuildMenuOverlay->OnExitBuildMode();
	}
}

FReply AEditorHUD::OnSaveTrack(const FString& TrackName)
{
	if (EditorController.IsValid())
	{
		EditorController->InputSaveTrack(TrackName);
	}

	return FReply::Handled();
}

FReply AEditorHUD::OnMenuSelected(UEditorBuildMenuDataAsset* Menu)
{
	if (EditorController.IsValid() && Menu != nullptr)
	{
		EditorController->InputSelectMenu(Menu);
	}
	return FReply::Handled();
}

FReply AEditorHUD::OnBlockSelected(const int8 Slot)
{
	if (EditorController.IsValid())
	{
		EditorController->InputSlot(Slot);
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

	if (Controller == nullptr)
	{
		return;
	}

	EditorController = Controller;
	
	if (GEngine == nullptr)
	{
		return;
	}

	EditorBuildMenuOverlay = SNew(SEditorBuildMenuOverlay)
		.Menu(CurrentActiveMenu)
		.OnMenuSelected_Lambda([this](UEditorBuildMenuDataAsset* Menu)
		{
			if (this && Menu)
			{
				return OnMenuSelected(Menu);
			}

			return FReply::Unhandled();
		})
		.OnBlockSelected_Lambda([this](const int8 Slot)
		{
			if (this)
			{
				return OnBlockSelected(Slot);
			}

			return FReply::Unhandled();
		});
	if (EditorBuildMenuOverlay.IsValid())
	{
		GEngine->GameViewport->AddViewportWidgetContent(EditorBuildMenuOverlay.ToSharedRef(), 5);
	}

	if (Controller)
	{
		Controller->OnMenuSlotSelected.AddDynamic(this, &AEditorHUD::OnMenuSlotSelected);
		Controller->OnExitBuildMode.AddDynamic(this, &AEditorHUD::OnExitBuildMode);
	}
}
