// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorHUD.h"

#include "SpinningWheels/Controllers/EditorController.h"
#include "UI/Slate/Overlays/EditorActions/EditorActionsOverlay.h"
#include "UI/Slate/Overlays/EditorBuildMenu/EditorBuildMenuOverlay.h"
#include "UI/Slate/Styles/MainStyle.h"

void AEditorHUD::BeginPlay()
{
	Super::BeginPlay();

	EditorController = Cast<AEditorController>(GetOwningPlayerController());

	InitializeOverlays();
	InitializeDelegates();
}

void AEditorHUD::InitializeOverlays()
{
	if (GEngine == nullptr)
	{
		return;
	}

	RootOverlay = SNew(SOverlay)
		.Visibility(EVisibility::SelfHitTestInvisible);
	if (RootOverlay.IsValid() == false)
	{
		return;
	}

	RootCanvas = SNew(SConstraintCanvas);
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootOverlay->AddSlot()
	           .Padding(FMainStyle::Get().GetMargin("Padding.SafeArea"))
	[
		RootCanvas.ToSharedRef()
	];

	GEngine->GameViewport->AddViewportWidgetContent(RootOverlay.ToSharedRef());

	InitializeOverlayEditorActions();
	InitializeOverlayBuildMenu();
}

void AEditorHUD::InitializeOverlayEditorActions()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	// Bottom right: editor buttons
	RootCanvas->AddSlot()
	          .Anchors(FAnchors(1.f, 1.f))
	          .Alignment(FVector2D(1.f, 1.f))
	          .AutoSize(true)
	[
		SAssignNew(EditorActionsOverlay, SEditorActionsOverlay)
		.OnSaveTrack_Lambda([this](const FString& TrackName)
		{
			if (this)
			{
				return OnSaveTrack(TrackName);
			}

			return FReply::Unhandled();
		})
	];
}

void AEditorHUD::InitializeOverlayBuildMenu()
{
	if (RootCanvas.IsValid() == false || EditorController.IsValid() == false)
	{
		return;
	}

	const FEditorBuildMenu CurrentActiveMenu = EditorController->GetCurrentActiveMenu();

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
		// Bottom center: build menu
		RootCanvas->AddSlot()
		          .Anchors(FAnchors(0.5f, 1.f))
		          .Alignment(FVector2D(.5f, 1.f))
		          .AutoSize(true)
		[
			EditorBuildMenuOverlay.ToSharedRef()
		];
	}

	EditorController->OnMenuSlotSelected.AddDynamic(this, &AEditorHUD::OnMenuSlotSelected);
	EditorController->OnExitBuildMode.AddDynamic(this, &AEditorHUD::OnExitBuildMode);
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
