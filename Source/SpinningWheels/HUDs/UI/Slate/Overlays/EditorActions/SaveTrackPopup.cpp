#include "SaveTrackPopup.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SSaveTrackPopup::Construct(const FArguments& InArgs)
{
	OnConfirmSaveTrack = InArgs._OnConfirmSaveTrack;
	OnCancelSaveTrack = InArgs._OnCancelSaveTrack;

	ChildSlot[

		SAssignNew(MainBox, SBox)
		.Visibility(EVisibility::Collapsed)
		.MinDesiredWidth(300.f)
		[
			SNew(SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Black"))
			.Padding(FMargin(10.f))
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(FMainStyle::Get().GetFontStyle("Font.Roboto.Regular.h4"))
					.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Primary.Light"))
					.Text(FText::FromString("Track name"))
				]

				+ SVerticalBox::Slot()
				[
					SAssignNew(TrackNameEditBox, SEditableTextBox)
					.Font(FMainStyle::Get().GetFontStyle("Font.Roboto.Regular.p"))

				]

				+ SVerticalBox::Slot()
				[

					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.Text(FText::FromString("Confirm"))
						.OnClicked(this, &SSaveTrackPopup::ExecuteConfirmSaveTrack)
					]

					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.Text(FText::FromString("Cancel"))
						.OnClicked(this, &SSaveTrackPopup::ExecuteCancelSaveTrack)
					]
				]
			]
		]


	];
}

FReply SSaveTrackPopup::ExecuteConfirmSaveTrack()
{
	if (TrackNameEditBox.IsValid())
	{
		if (OnConfirmSaveTrack.IsBound())
		{
			FReply Reply = OnConfirmSaveTrack.Execute(TrackNameEditBox->GetText().ToString());
		}
	}
	return FReply::Unhandled();
}

FReply SSaveTrackPopup::ExecuteCancelSaveTrack()
{
	if (OnCancelSaveTrack.IsBound())
	{
		FReply Reply = OnCancelSaveTrack.Execute();
	}

	return FReply::Unhandled();
}

void SSaveTrackPopup::Show()
{
	if (MainBox.IsValid())
	{
		MainBox->SetVisibility(EVisibility::Visible);
	}
}

void SSaveTrackPopup::Hide()
{
	if (MainBox.IsValid())
	{
		MainBox->SetVisibility(EVisibility::Collapsed);
	}
}
