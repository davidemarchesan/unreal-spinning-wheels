// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "UI/Slate/Pages/MainMenu/MainMenuPage.h"
#include "UI/Slate/Pages/Tracks/TracksPage.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeMainHUD();
}

void AMainHUD::InitializeMainHUD()
{

	InitializeLoadingOverlay();

	// todo: init root widget switcher when everything is loaded
	// bRootInitialized = InitializeRootOverlay();
	//
	// if (bRootInitialized)
	// {
	// 	InitializePages();
	// }
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

	TracksPage = SNew(STracksPage)
		.OnPageBack_Lambda([this]()
		{
			HandleBack();
			return FReply::Handled();
		});

	if (MainSwitcher.IsValid())
	{
		MainSwitcher->AddSlot()[MainMenuPage.ToSharedRef()];
		MainSwitcher->AddSlot()[TracksPage.ToSharedRef()];

		// GoTo(EMenuPage::MP_None); // Should be this one
		GoTo(EMenuPage::MP_Tracks); // Testing
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
		MainSwitcher->SetActiveWidget(TracksPage.ToSharedRef());
		// FSlateApplication::Get().SetKeyboardFocus(TracksPage);
		if (TracksPage->GetFocusWidget().IsValid())
		{
			FSlateApplication::Get().SetKeyboardFocus(TracksPage->GetFocusWidget());
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
