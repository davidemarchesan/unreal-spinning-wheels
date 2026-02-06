// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Slate/Overlays/Loading/LoadingOverlay.h"
#include "UI/Slate/Pages/Pages.h"
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

private:

	void InitializeMainHUD();

	void InitializeLoadingOverlay();
	TSharedPtr<SLoadingOverlay> LoadingOverlay;

	bool bRootInitialized = false;
	bool InitializeRootOverlay();

	void InitializePages();

	// Main switcher
	TSharedPtr<SWidgetSwitcher> MainSwitcher;

	// Pages
	TSharedPtr<SWidget> MainMenuPage;
	TSharedPtr<STracksPage> TracksPage;

	void GoTo(const EMenuPage Page);
	void HandleBack();

	// History
	EMenuPage CurrentPage = EMenuPage::MP_None;
	TArray<EMenuPage> PagesHistory;
	
};
