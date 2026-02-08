#pragma once
#include "SpinningWheels/Core/Slate.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/PopupWidget.h"

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnConfirmSaveTrack, const FString& TrackName)

class SEditorSaveTrackPopup : public SPopupBase
{
public:
	SLATE_BEGIN_ARGS(SEditorSaveTrackPopup)
		{
		}
		
		SLATE_EVENT(FOnConfirmSaveTrack, OnConfirmSaveTrack)
		SLATE_EVENT(FOnReply, OnBack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SEditableTextBox> TrackNameEditBox;

	FOnConfirmSaveTrack OnConfirmSaveTrack;
	FOnReply OnBack;

	FReply ExecuteConfirmSaveTrack();

protected:

	virtual FReply ExecuteOnBack() override;

	virtual FReply ExecuteOnFocusReceive() override;

public:

	void SetTrackName(const FString& InTrackName);

};
