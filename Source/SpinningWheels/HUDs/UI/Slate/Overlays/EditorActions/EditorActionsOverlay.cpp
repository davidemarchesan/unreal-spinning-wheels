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
			.Text(FText::FromString("Save track"))
			.OnClicked(this, &SEditorActionsOverlay::ExecuteSaveTrack)
			.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Primary.Yellow"))
			.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Button.Primary.Yellow"))
		]

	];
}

FReply SEditorActionsOverlay::ExecuteSaveTrack()
{
	if (OnSaveTrack.IsBound())
	{
		FReply Reply = OnSaveTrack.Execute();
		return Reply;
	}
	
	return FReply::Unhandled();
}
