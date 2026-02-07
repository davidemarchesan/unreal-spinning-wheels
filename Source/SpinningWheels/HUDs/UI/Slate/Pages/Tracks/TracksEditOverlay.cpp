#include "TracksEditOverlay.h"

#include "TrackItemWidget.h"
#include "Widgets/Layout/SWrapBox.h"

void STracksEditOverlay::Construct(const FArguments& InArgs)
{
	Tracks = InArgs._Tracks;

	OnTrackSelected = InArgs._OnTrackSelected;
	OnTrackUnselected = InArgs._OnTrackUnselected;

	WrapBox = SNew(SWrapBox)
		.Orientation(Orient_Horizontal)
		.UseAllottedSize(true)
		.InnerSlotPadding(FVector2D(10.f))
		.HAlign(HAlign_Center);

	for (int32 i = 0; i < Tracks.Num(); i++)
	{
		TSharedPtr<STrackItem> Item = SNew(STrackItem)
			.Text(FText::FromString(Tracks[i].Name))
			.IsSelectable(true)
			.OnSelected_Lambda([this, i]()
			{
				ExecuteOnTrackSelected(i);
				return FReply::Handled();
			})
			.OnUnselected_Lambda([this, i]()
			{
				ExecuteOnTrackUnselected(i);
				return FReply::Handled();
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
		SNew(SBox)
		.VAlign(VAlign_Center)
		[

			WrapBox.ToSharedRef()
		]

	];
}

void STracksEditOverlay::ExecuteOnTrackSelected(const int32 Index)
{
	if (OnTrackSelected.IsBound())
	{
		OnTrackSelected.Execute(Index);
	}
}

void STracksEditOverlay::ExecuteOnTrackUnselected(const int32 Index)
{
	if (OnTrackUnselected.IsBound())
	{
		OnTrackUnselected.Execute(Index);
	}
}

void STracksEditOverlay::DeselectTracks(int32 Except)
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