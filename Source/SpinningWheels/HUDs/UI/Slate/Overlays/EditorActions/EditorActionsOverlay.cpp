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
			.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Primary"))
			.OnClicked(this, &SEditorActionsOverlay::ExecuteSaveTrack)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Save track"))
				.Font(FMainStyle::Get().GetFontStyle("Font.Orbitron.Medium.h4"))
				.Justification(ETextJustify::Center)
			]
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
