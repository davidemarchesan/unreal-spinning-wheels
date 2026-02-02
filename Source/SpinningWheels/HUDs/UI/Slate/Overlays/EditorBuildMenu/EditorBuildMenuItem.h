#pragma once

class SEditorBuildMenuItem : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SEditorBuildMenuItem) {}

		SLATE_ARGUMENT(int8, Slot)
		SLATE_ARGUMENT(FText, Name);
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SOverlay> MainOverlay;

	FButtonStyle ButtonStyle; // todo: put it on main style

public:

	void SetSelected();
	void SetUnselected();
	
};