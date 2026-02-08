#include "TracksMainOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Widgets/CardWidget.h"
#include "Widgets/Layout/SWrapBox.h"

void STracksMainOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SNew(SBox)
		.VAlign(VAlign_Center)
		[
			SNew(SWrapBox)
			.Orientation(Orient_Horizontal)
			.UseAllottedSize(true)
			.InnerSlotPadding(FVector2D(10.f))
			.HAlign(HAlign_Center)

			+ SWrapBox::Slot()
			[
				SNew(SCard)
				.Text(FText::FromString("Create track"))
				.OnClicked(InArgs._OnCreateTrack)

			]

			+ SWrapBox::Slot()
			[
				SNew(SCard)
				.Text(FText::FromString("Edit track"))
				.OnClicked(InArgs._OnGoToTracks)
			]
		]
	];
}
