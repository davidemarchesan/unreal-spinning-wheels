#include "TracksPage.h"

#include "TrackItemWidget.h"
#include "SpinningWheels/HUDs/UI/Slate/Pages/PageBase.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"
#include "Widgets/Layout/SWrapBox.h"

void STracksPage::Construct(const FArguments& InArgs)
{
	OnPageBack = InArgs._OnPageBack;

	TArray<FString> Tracks = {
		FString("Track 1"),
		FString("Track 2"),
		FString("Track 3"),
		FString("Track 4"),
		FString("Track 5"),
		FString("Track 6"),
		FString("Track 7"),
	};

	WrapBox = SNew(SWrapBox)
		.Orientation(Orient_Horizontal)
		.UseAllottedSize(true)
		.InnerSlotPadding(FVector2D(10.f))
		.HAlign(HAlign_Center);

	for (int32 i = 0; i < Tracks.Num(); i++)
	{
		TSharedPtr<STrackItem> Item = SNew(STrackItem)
			.Text(FText::FromString(Tracks[i]))
			.IsSelectable(true)
			.OnSelected_Lambda([this, i]()
			{
				OnTrackSelected(i);
			})
			.OnUnselected_Lambda([this, i]()
			{
				OnTrackUnselected(i);
			});

		TrackItems.Push(Item);

		if (i == 0)
		{
			DefaultTrackItem = Item;
		}

		WrapBox->AddSlot()
		[
			Item.ToSharedRef()
		];
	}

	ChildSlot[

		SNew(SPageBase)
		.PageTitleText(FText::FromString("Tracks"))
		.BodySlot()
		[
			SNew(SBox)
			[
				SNew(SBorder) // todo: remove, just for testing
				[
					WrapBox.ToSharedRef()
				]
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
			]
		]

	];
}

void STracksPage::OnTrackSelected(const int32 Index)
{
	// Unselect all other tracks (visual only)
	DeselectTracks(Index);
	
	SetSelectedTrackIndex(Index);
}

void STracksPage::OnTrackUnselected(const int32 Index)
{
	if (SelectedTrackIndex == Index)
	{
		SetSelectedTrackIndex(INDEX_NONE);
	}
}

void STracksPage::DeselectTracks(int32 Except)
{
	for (int32 i = 0; i < TrackItems.Num(); i++)
	{
		if (i != Except)
		{
			if (TrackItems[i].IsValid())
			{
				TrackItems[i]->SetSelected(false);
			}
		}
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

bool STracksPage::SupportsKeyboardFocus() const
{
	return true;
}

FReply STracksPage::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Back)
	{
		if (SelectedTrackIndex != INDEX_NONE)
		{
			DeselectTracks();
			SetSelectedTrackIndex(INDEX_NONE);
			return FReply::Handled();
		}

		if (OnPageBack.IsBound())
		{
			return OnPageBack.Execute();
		}
	}

	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}
