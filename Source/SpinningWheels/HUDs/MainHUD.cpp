// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "Kismet/GameplayStatics.h"
#include "SpinningWheels/Subsystems/GameInstance/LoadingSubsystem.h"
#include "SpinningWheels/Subsystems/GameInstance/TrackEditorSubsystem.h"
#include "SpinningWheels/Subsystems/GameInstance/TracksSubsystem.h"
#include "UI/Slate/Pages/MainMenu/MainMenuPage.h"
#include "UI/Slate/Pages/Tracks/TracksPage.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeMainHUD();
}

void AMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnLoadingSubsystemReadyDelegateHandle.IsValid())
	{
		OnLoadingSubsystemReadyDelegateHandle.Reset();
	}
}

void AMainHUD::InitializeMainHUD()
{
	InitializeLoadingOverlay();

	GameInstance = GetGameInstance();
	if (GameInstance.IsValid())
	{
		LoadingSubsystem = GameInstance->GetSubsystem<ULoadingSubsystem>();
		TracksSubsystem = GameInstance->GetSubsystem<UTracksSubsystem>();
		TrackEditorSubsystem = GameInstance->GetSubsystem<UTrackEditorSubsystem>();

		if (LoadingSubsystem.IsValid())
		{
			if (LoadingSubsystem.Get()->IsReady())
			{
				OnLoadingSubsystemReady();
			}
			else
			{
				LoadingSubsystem->OnLoadingSubsystemReady.AddUObject(this, &AMainHUD::OnLoadingSubsystemReady);
			}
		}
	}
}

void AMainHUD::InitializeLoadingOverlay()
{
	if (GEngine == nullptr)
	{
		return;
	}

	if (GEngine->GameViewport == nullptr)
	{
		return;
	}

	LoadingOverlay = SNew(SLoadingOverlay);

	GEngine->GameViewport->AddViewportWidgetContent(LoadingOverlay.ToSharedRef(), 100);
}

bool AMainHUD::InitializeRootOverlay()
{
	if (GEngine == nullptr)
	{
		return false;
	}

	if (GEngine->GameViewport == nullptr)
	{
		return false;
	}

	MainSwitcher = SNew(SWidgetSwitcher);

	GEngine->GameViewport->AddViewportWidgetContent(MainSwitcher.ToSharedRef());

	return true;
}

void AMainHUD::InitializePages()
{
	MainMenuPage = SNew(SMainMenuPage)
		.OnMainMenuItemSelected_Lambda([this](const EMenuPage Page)
		{
			if (this)
			{
				GoTo(Page);
			}
			return FReply::Handled();
		});

	if (TracksSubsystem.IsValid())
	{
		TracksPage = SNew(STracksPage)
			.Tracks(TracksSubsystem->GetTracks())
			.OnCreateTrack_Lambda([]()
			{
			})
			.OnEditTrack_Lambda([this](const FTrackSaveData& Track)
			{
				OnEditTrack(Track);
			})
			.OnPageBack_Lambda([this]()
			{
				HandleBack();
				return FReply::Handled();
			});
	}

	if (MainSwitcher.IsValid())
	{
		MainSwitcher->AddSlot()[MainMenuPage.ToSharedRef()];
		MainSwitcher->AddSlot()[TracksPage.ToSharedRef()];

		GoTo(EMenuPage::MP_None); // Should be this one
	}
}

void AMainHUD::GoTo(const EMenuPage Page)
{
	PagesHistory.Add(CurrentPage);
	CurrentPage = Page;

	switch (Page)
	{
	case EMenuPage::MP_Play:
		break;

	case EMenuPage::MP_Tracks:
		{
			MainSwitcher->SetActiveWidget(TracksPage.ToSharedRef());
			if (TracksPage->GetFocusWidget().IsValid())
			{
				FSlateApplication::Get().SetKeyboardFocus(TracksPage->GetFocusWidget());
			}
		}
		break;

	default:
		MainSwitcher->SetActiveWidget(MainMenuPage.ToSharedRef());
		FSlateApplication::Get().SetKeyboardFocus(MainMenuPage);
		break;
	}
}

void AMainHUD::HandleBack()
{
	if (PagesHistory.Num() > 0)
	{
		const EMenuPage PreviousPage = PagesHistory.Top();
		PagesHistory.Pop(EAllowShrinking::No);
		GoTo(PreviousPage);

		return;
	}

	GoTo(EMenuPage::MP_None);
}

void AMainHUD::OnCreateTrack()
{
}

void AMainHUD::OnEditTrack(const FTrackSaveData& Track)
{
	if (TrackEditorSubsystem.IsValid())
	{
		TrackEditorSubsystem->SetNextTrackToLoad(Track);

		GEngine->GameViewport->RemoveAllViewportWidgets();
		UGameplayStatics::OpenLevel(GetWorld(),"L_Editor");
	}
}

void AMainHUD::OnLoadingSubsystemReady()
{
	bRootInitialized = InitializeRootOverlay();

	if (bRootInitialized)
	{
		InitializePages();

		GEngine->GameViewport->RemoveViewportWidgetContent(LoadingOverlay.ToSharedRef());
	}
}
