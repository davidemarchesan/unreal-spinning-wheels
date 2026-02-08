#include "EditorSaveTrackPopup.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorSaveTrackPopup::Construct(const FArguments& InArgs)
{
	OnConfirmSaveTrack = InArgs._OnConfirmSaveTrack;
	OnBack = InArgs._OnBack;

	ChildSlot[
		SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[

			SNew(SBox)
			.MinDesiredWidth(300.f)
			[
				SNew(SBorder)
				.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
				.Padding(FMainStyle::Get().GetMargin("Padding.Box"))
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[

						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.P"))
							.Text(FText::FromString("Track name"))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(TrackNameEditBox, SEditableTextBox)
							.Style(&FMainStyle::Get().GetWidgetStyle<FEditableTextBoxStyle>("Text.Editable"))
						]

					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SSpacer)
						.Size(FVector2D(1.f, 10.f))
					]

					+ SVerticalBox::Slot()
					[

						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
							.Text(FText::FromString("Confirm"))
							.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Primary"))
							.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Button.Primary"))
							.HAlign(HAlign_Center)
							.OnClicked(this, &SEditorSaveTrackPopup::ExecuteConfirmSaveTrack)
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSpacer)
							.Size(FVector2D(10.f, 1.f))
						]

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
							.Text(FText::FromString("Cancel"))
							.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Secondary"))
							.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Button.Secondary"))
							.HAlign(HAlign_Center)
							.OnClicked(InArgs._OnBack)
						]

					]
				]
			]
		]
	];

	
}

FReply SEditorSaveTrackPopup::ExecuteConfirmSaveTrack()
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

FReply SEditorSaveTrackPopup::ExecuteOnBack()
{
	if (OnBack.IsBound())
	{
		return OnBack.Execute();
	}
	return FReply::Unhandled();
}

FReply SEditorSaveTrackPopup::ExecuteOnFocusReceive()
{
	if (TrackNameEditBox.IsValid())
	{
		FSlateApplication::Get().SetKeyboardFocus(TrackNameEditBox.ToSharedRef());
	}
	return FReply::Handled();
}

void SEditorSaveTrackPopup::SetTrackName(const FString& InTrackName)
{
	if (TrackNameEditBox.IsValid())
	{
		TrackNameEditBox->SetText(FText::FromString(InTrackName));
	}
}
