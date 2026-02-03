#include "SaveTrackPopup.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SSaveTrackPopup::Construct(const FArguments& InArgs)
{
	OnConfirmSaveTrack = InArgs._OnConfirmSaveTrack;
	OnCancelSaveTrack = InArgs._OnCancelSaveTrack;

	Temp = FTextBlockStyle();
	Temp.SetColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"));
	Temp.SetFont(FMainStyle::Get().GetFontStyle("Font.Funnel.Regular.h4"));

	ChildSlot[

		SAssignNew(MainBox, SBox)
		// .Visibility(EVisibility::Collapsed)
		.MinDesiredWidth(300.f)
		[
			SNew(SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
			.Padding(FMainStyle::Get().GetMargin("Padding.Box"))
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Track name"))
					.TextStyle(&Temp)
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
