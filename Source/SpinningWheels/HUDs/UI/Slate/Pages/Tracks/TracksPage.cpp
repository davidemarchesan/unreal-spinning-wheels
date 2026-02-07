#include "TracksPage.h"

#include "SpinningWheels/HUDs/UI/Slate/Pages/PageBase.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void STracksPage::Construct(const FArguments& InArgs)
{
	Tracks = InArgs._Tracks;

	OnEditTrack = InArgs._OnEditTrack;
	OnPageBack = InArgs._OnPageBack;

	ChildSlot[

		SNew(SPageBase)
		.PageTitleText(FText::FromString("Tracks"))
		.BodySlot()
		[
			SAssignNew(MainSwitcher, SWidgetSwitcher)

			+ SWidgetSwitcher::Slot()
			[
				SAssignNew(TracksMainOverlay, STracksMainOverlay)
				.OnCreateTrack(InArgs._OnCreateTrack)
				.OnGoToTracks(this, &STracksPage::GoToTracksSubPage)
			]

			+ SWidgetSwitcher::Slot()
			[
				SAssignNew(TracksEditOverlay, STracksEditOverlay)
				.Tracks(InArgs._Tracks)
				.OnTrackSelected(this, &STracksPage::OnTrackSelected)
				.OnTrackUnselected(this, &STracksPage::OnTrackUnselected)
			]
		]
		.ContextActionsSlot()
		[
			SAssignNew(ContextActionsBox, SBox)
			.Visibility(EVisibility::Collapsed)
			[
				SNew(SButton)
				.Text(FText::FromString("Edit"))
				.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Primary"))
				.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Button.Primary"))
				.OnClicked(this, &STracksPage::ExecuteEditTrack)
			]
		]

	];

	if (MainSwitcher.IsValid() && TracksMainOverlay.IsValid())
	{
		MainSwitcher->SetActiveWidget(TracksMainOverlay.ToSharedRef());
	}
}

FReply STracksPage::GoToTracksSubPage()
{
	if (MainSwitcher.IsValid() && TracksEditOverlay.IsValid())
	{
		MainSwitcher->SetActiveWidget(TracksEditOverlay.ToSharedRef());
		bInSubPage = true;

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void STracksPage::OnTrackSelected(const int32 Index)
{
	// Unselect all other tracks (visual only)
	if (TracksEditOverlay.IsValid())
	{
		TracksEditOverlay->DeselectTracks(Index);
	}

	SetSelectedTrackIndex(Index);
}

void STracksPage::OnTrackUnselected(const int32 Index)
{
	if (SelectedTrackIndex == Index)
	{
		SetSelectedTrackIndex(INDEX_NONE);
	}
}

void STracksPage::UpdateContextActions()
{
	if (ContextActionsBox.IsValid())
	{
		ContextActionsBox->SetVisibility(SelectedTrackIndex == INDEX_NONE
			                                 ? EVisibility::Collapsed
			                                 : EVisibility::Visible);
	}
}

void STracksPage::SetSelectedTrackIndex(const int32 Index)
{
	SelectedTrackIndex = Index;
	UpdateContextActions();
}

FReply STracksPage::ExecuteEditTrack()
{
	if (Tracks.IsValidIndex(SelectedTrackIndex) && OnEditTrack.IsBound())
	{
		OnEditTrack.Execute(Tracks[SelectedTrackIndex]);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

bool STracksPage::SupportsKeyboardFocus() const
{
	return true;
}

FReply STracksPage::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Back)
	{
		if (bInSubPage == true)
		{
			if (MainSwitcher.IsValid() && TracksMainOverlay.IsValid() && TracksEditOverlay.IsValid())
			{
				if (SelectedTrackIndex != INDEX_NONE)
				{
					TracksEditOverlay->DeselectTracks();
					SetSelectedTrackIndex(INDEX_NONE);
					return FReply::Handled();
				}
				else
				{
					MainSwitcher->SetActiveWidget(TracksMainOverlay.ToSharedRef());
					bInSubPage = false;
					return FReply::Handled();
				}
			}
			return FReply::Unhandled();
		}
		else if (OnPageBack.IsBound())
		{
			return OnPageBack.Execute();
		}
	}

	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}
