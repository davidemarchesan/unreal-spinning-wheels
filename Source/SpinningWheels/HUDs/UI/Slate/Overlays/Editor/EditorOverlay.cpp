#include "EditorOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorOverlay::Construct(const FArguments& InArgs)
{

	OnSaveTrack = InArgs._OnSaveTrack;
	
	ChildSlot[

		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(20.f)
		[

			SNew(SVerticalBox)

			// + SVerticalBox::Slot()
			// [
			// 	SNew(SButton)
			// 	.Text(FText::FromString("Test & Play"))
			// ]

			+ SVerticalBox::Slot()
			[
				SNew(SButton)
				.Text(FText::FromString("Save"))
			]

		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Black"))
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				[
					SAssignNew(TrackNameEditBox, SEditableTextBox)
				]

				+ SVerticalBox::Slot()
				[
					SNew(SButton)
					.Text(FText::FromString("Save"))
					.OnClicked(this, &SEditorOverlay::ExecuteSaveTrack)
				]

				+ SVerticalBox::Slot()
				[
					SNew(SButton)
					.Text(FText::FromString("Cancel"))
				]
			]
		]

	];
}

FReply SEditorOverlay::ExecuteSaveTrack()
{
	if (TrackNameEditBox.IsValid())
	{
		if (OnSaveTrack.IsBound())
		{
			const FString TrackName = TrackNameEditBox->GetText().ToString();
			FReply Reply = OnSaveTrack.Execute(TrackName);

			return Reply;
		}
		
	}
	return FReply::Unhandled();
}
