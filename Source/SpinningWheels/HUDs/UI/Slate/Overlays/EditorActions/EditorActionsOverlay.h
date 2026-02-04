#pragma once

DECLARE_DELEGATE_RetVal(FReply, FOnTestTrack)
DECLARE_DELEGATE_RetVal(FReply, FOnSaveTrack)

class SEditorActionsOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditorActionsOverlay)
		{
		}

		SLATE_EVENT(FOnTestTrack, OnTestTrack)
		SLATE_EVENT(FOnSaveTrack, OnSaveTrack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	
	FOnTestTrack OnTestTrack;
	FOnSaveTrack OnSaveTrack;
	
	FReply ExecuteTestTrack();
	FReply ExecuteSaveTrack();

public:
};
