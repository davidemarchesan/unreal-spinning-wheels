// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorHUD.h"

#include "Kismet/GameplayStatics.h"
#include "SpinningWheels/Controllers/EditorController.h"
#include "SpinningWheels/GameModes/EditorGameMode.h"
#include "UI/Slate/Overlays/EditorActions/EditorActionsOverlay.h"
#include "UI/Slate/Overlays/EditorActions/SaveTrackPopup.h"
#include "UI/Slate/Overlays/EditorBuildMenu/EditorBuildMenuOverlay.h"
#include "UI/Slate/Overlays/EditorMenu/EditorMenuPopup.h"
#include "UI/Slate/Overlays/EditorTrackData/EditorTrackDataOverlay.h"
#include "UI/Slate/Styles/MainStyle.h"
#include "UI/Slate/Widgets/Modals/ModalConfirmWidget.h"

void AEditorHUD::BeginPlay()
{
	Super::BeginPlay();

	EditorGameMode = GetWorld()->GetAuthGameMode<AEditorGameMode>();
	EditorController = Cast<AEditorController>(GetOwningPlayerController());

	InitializeRootOverlay();
	InitializeDelegates();
}

void AEditorHUD::InputOpenMenu()
{
	if (EditorMenuPopup)
	{
		ShowModalOverlay(EditorMenuPopup);
		FSlateApplication::Get().SetKeyboardFocus(EditorMenuPopup.ToSharedRef());
	}
}

void AEditorHUD::InitializeRootOverlay()
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

	ModalOverlay = SNew(SOverlay)
		.Visibility(EVisibility::Collapsed);

	GEngine->GameViewport->AddViewportWidgetContent(ModalOverlay.ToSharedRef(), 5);

	InitializeOverlayEditorActions();
	InitializeOverlayEditorBuildMenu();
	InitializeOverlayEditorTrackData();
	InitializeOverlayEditorSavePopup();
	InitializeOverlayEditorMenu();
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
		.OnTestTrack_Lambda([this]()
		{
			if (this)
			{
				return OnTestTrack();
			}

			return FReply::Unhandled();
		})
		.OnSaveTrack_Lambda([this]()
		{
			if (this)
			{
				return OnSaveTrack();
			}

			return FReply::Unhandled();
		})
	];
}

void AEditorHUD::InitializeOverlayEditorBuildMenu()
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
	// todo: transform to public method, avoid using delegate
	EditorController->OnExitBuildMode.AddDynamic(this, &AEditorHUD::OnExitBuildMode);
	// todo: transform to public method, avoid using delegate
}

void AEditorHUD::InitializeOverlayEditorTrackData()
{
	if (RootCanvas.IsValid() == false)
	{
		return;
	}

	RootCanvas->AddSlot()
	          .Anchors(FAnchors(0.f, 0.f))
	          .Alignment(FVector2D(0.f, 0.f))
	          .AutoSize(true)
	[
		SAssignNew(EditorTrackDataOverlay, SEditorTrackDataOverlay)
	];
}

void AEditorHUD::InitializeOverlayEditorSavePopup()
{
	if (ModalOverlay.IsValid() == false)
	{
		return;
	}

	SaveTrackPopup = SNew(SSaveTrackPopup)
		.OnConfirmSaveTrack_Lambda([this](const FString& TrackName)
		{
			if (this)
			{
				OnConfirmSaveTrack(TrackName);
			}
			return FReply::Handled();
		})
		.OnCancelSaveTrack_Lambda([this]()
		{
			if (this)
			{
				OnCancelSaveTrack();
			}

			return FReply::Handled();
		});
}

void AEditorHUD::InitializeOverlayEditorMenu()
{
	if (ModalOverlay.IsValid() == false)
	{
		return;
	}

	EditorMenuPopup = SNew(SEditorMenuPopup)
		.OnBack_Lambda([this]()
		{
			if (this)
			{
				HideModalOverlay();
			}
			return FReply::Handled();
		})
		.OnSaveTrack_Lambda([this]()
		{
			if (this)
			{
				HideModalOverlay();
				return OnSaveTrack();
			}

			return FReply::Unhandled();
		})
		.OnTestTrack_Lambda([this]()
		{
			if (this)
			{
				HideModalOverlay();
				return OnTestTrack();
			}

			return FReply::Unhandled();
		})
		.OnGoToMainMenu_Lambda([this]()
		{
			if (this)
			{
				return OnGoToMainMenu();
			}
			return FReply::Unhandled();
		});
}

void AEditorHUD::InitializeDelegates()
{
	if (EditorGameMode.IsValid())
	{
		EditorGameMode->OnTrackSaved.AddDynamic(this, &AEditorHUD::OnTrackSaved); // todo: remove binding
	}
}

void AEditorHUD::ShowModalOverlay(const TSharedPtr<SWidget>& Widget)
{
	if (ModalOverlay.IsValid() && Widget.IsValid())
	{
		ModalOverlay->ClearChildren();
		ModalOverlay->AddSlot()
		            .VAlign(VAlign_Fill)
		            .HAlign(HAlign_Fill)
		[
			Widget.ToSharedRef()
		];

		if (EditorController.IsValid())
		{
			EditorController->BlockCursor();
		}

		ModalOverlay->SetVisibility(EVisibility::Visible);
	}
}

void AEditorHUD::HideModalOverlay()
{
	if (ModalOverlay.IsValid())
	{
		ModalOverlay->SetVisibility(EVisibility::Collapsed);
		ModalOverlay->ClearChildren();

		if (EditorController.IsValid())
		{
			EditorController->UnlockCursor();
		}
	}
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

void AEditorHUD::OnTrackSaved(const FTrack& CurrentTrack, const bool bSuccess)
{
	TSharedPtr<SModalConfirm> ModalConfirm = SNew(SModalConfirm)
		.Text(FText::FromString(bSuccess
			                        ? "Track saved successfully."
			                        : "An error occured while saving the track. Please try again."))
		.OnConfirm_Lambda([this]()
		{
			HideModalOverlay();
			return FReply::Handled();
		});

	ShowModalOverlay(ModalConfirm);

	if (EditorTrackDataOverlay.IsValid())
	{
		EditorTrackDataOverlay->Update(CurrentTrack);
	}
}

FReply AEditorHUD::OnTestTrack()
{
	if (EditorController.IsValid())
	{
		EditorController->InputTestTrack();
	}
	return FReply::Handled();
}

FReply AEditorHUD::OnSaveTrack()
{
	FString CurrentTrackName = "";
	if (EditorController.IsValid())
	{
		CurrentTrackName = EditorController->GetTrackName();
	}

	if (ModalOverlay.IsValid() && SaveTrackPopup.IsValid())
	{
		SaveTrackPopup->SetTrackName(CurrentTrackName);
		ShowModalOverlay(SaveTrackPopup);
	}
	return FReply::Handled();
}

FReply AEditorHUD::OnConfirmSaveTrack(const FString& TrackName)
{
	TSharedPtr<SModalBase> ModalSaving = SNew(SModalBase)
		.BodySlot()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Saving..."))
			.Justification(ETextJustify::Center)
			.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.P"))
		];
	ShowModalOverlay(ModalSaving);

	if (EditorController.IsValid())
	{
		EditorController->InputSaveTrack(TrackName);
	}

	return FReply::Handled();
}

FReply AEditorHUD::OnCancelSaveTrack()
{
	if (SaveTrackPopup.IsValid())
	{
		SaveTrackPopup->Hide();
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

FReply AEditorHUD::OnGoToMainMenu()
{
	GEngine->GameViewport->RemoveAllViewportWidgets();
	UGameplayStatics::OpenLevel(GetWorld(), "L_Main");
	return FReply::Handled();
}
