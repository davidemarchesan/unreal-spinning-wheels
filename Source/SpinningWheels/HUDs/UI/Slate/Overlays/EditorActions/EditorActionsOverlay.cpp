#include "EditorActionsOverlay.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorActionsOverlay::Construct(const FArguments& InArgs)
{
	OnTestTrack = InArgs._OnTestTrack;
	OnSaveTrack = InArgs._OnSaveTrack;
	
	ChildSlot[

		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("Test track"))
			.OnClicked(this, &SEditorActionsOverlay::ExecuteTestTrack)
			.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Primary.Large"))
			.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Button.Primary.Large"))
		]

		+ SVerticalBox::Slot()
		[
			SNew(SSpacer)
			.Size(FVector2D(1.f, 10.f))
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("Save track"))
			.OnClicked(this, &SEditorActionsOverlay::ExecuteSaveTrack)
			.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Primary.Large"))
			.TextStyle(&FMainStyle::Get().GetWidgetStyle<FTextBlockStyle>("Text.Button.Primary.Large"))
		]

	];
}

FReply SEditorActionsOverlay::ExecuteTestTrack()
{
	if (OnTestTrack.IsBound())
	{
		FReply Reply = OnTestTrack.Execute();
		return Reply;
	}
	return FReply::Unhandled();
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
