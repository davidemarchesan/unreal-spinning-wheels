#pragma once

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnSaveTrack, const FString& Name)

class SEditorOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditorOverlay)
		{
		}

		SLATE_EVENT(FOnSaveTrack, OnSaveTrack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SEditableTextBox> TrackNameEditBox;

	FOnSaveTrack OnSaveTrack;
	
	FReply ExecuteSaveTrack();

public:
};
