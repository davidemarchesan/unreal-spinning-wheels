#include "EditorActionsOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorActionsOverlay::Construct(const FArguments& InArgs)
{
	OnSaveTrack = InArgs._OnSaveTrack;

	ChildSlot[

		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Save"))
		]

	];
}

FReply SEditorActionsOverlay::ExecuteSaveTrack()
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
