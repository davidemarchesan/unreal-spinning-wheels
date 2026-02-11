// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Slate/Overlays/Loading/LoadingOverlay.h"
#include "UI/Slate/Pages/Pages.h"
#include "UI/Slate/Pages/Play/PlayPage.h"
#include "UI/Slate/Pages/Tracks/TracksPage.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGWHEELS_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	void InitializeMainHUD();

	TWeakObjectPtr<class AMainController> MainController;
	
	// Subsystems
	TWeakObjectPtr<class UGameInstance> GameInstance;
	TWeakObjectPtr<class ULoadingSubsystem> LoadingSubsystem;
	TWeakObjectPtr<class UTracksSubsystem> TracksSubsystem;
	TWeakObjectPtr<class UTrackEditorSubsystem> TrackEditorSubsystem;
	TWeakObjectPtr<class URaceServerSubsystem> RaceServerSubsystem;
	
	void InitializeLoadingOverlay();
	TSharedPtr<SLoadingOverlay> LoadingOverlay;

	// Main overlay
	bool bRootInitialized = false;
	bool InitializeRootOverlay();

	void InitializePages();

	// Main switcher
	TSharedPtr<SWidgetSwitcher> MainSwitcher;

	// Pages
	TSharedPtr<SWidget> MainMenuPage;
	TSharedPtr<SPlayPage> PlayPage;
	TSharedPtr<STracksPage> TracksPage;

	void GoTo(const EMenuPage Page);
	void HandleBack();

	void OnCreateTrack();
	void OnEditTrack(const FTrackSaveData& Track);

	void OnStartLANSession();
	void OnJoinLANSession();
	void OnStartSteamSession();

	// History
	EMenuPage CurrentPage = EMenuPage::MP_None;
	TArray<EMenuPage> PagesHistory;

	// Delegates
	void OnLoadingSubsystemReady();
	FDelegateHandle OnLoadingSubsystemReadyDelegateHandle;
	
};
