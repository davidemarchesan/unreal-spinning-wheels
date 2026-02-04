#pragma once

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnConfirmSaveTrack, const FString& TrackName)
DECLARE_DELEGATE_RetVal(FReply, FOnCancelSaveTrack)

class SSaveTrackPopup : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSaveTrackPopup)
		{
		}
		
		SLATE_EVENT(FOnConfirmSaveTrack, OnConfirmSaveTrack)
		SLATE_EVENT(FOnCancelSaveTrack, OnCancelSaveTrack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SBox> MainBox;
	TSharedPtr<SEditableTextBox> TrackNameEditBox;

	FOnConfirmSaveTrack OnConfirmSaveTrack;
	FOnCancelSaveTrack OnCancelSaveTrack;

	FReply ExecuteConfirmSaveTrack();
	FReply ExecuteCancelSaveTrack();

public:

	void Show();
	void Hide();
};
