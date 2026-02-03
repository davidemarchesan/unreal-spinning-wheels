#pragma once

DECLARE_DELEGATE_RetVal(FReply, FOnSaveTrack)

class SEditorActionsOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditorActionsOverlay)
		{
		}

		SLATE_EVENT(FOnSaveTrack, OnSaveTrack)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	
	FOnSaveTrack OnSaveTrack;
	
	FReply ExecuteSaveTrack();

public:
};
