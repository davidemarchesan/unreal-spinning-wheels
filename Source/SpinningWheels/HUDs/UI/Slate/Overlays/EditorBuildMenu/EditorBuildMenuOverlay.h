#pragma once

#include "EditorBuildMenu.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"

class SEditorBuildMenuOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SEditorBuildMenuOverlay) {}
		SLATE_ARGUMENT(FEditorBuildMenu, Menu)

		SLATE_EVENT(FOnMenuSelected, OnMenuSelected)
		SLATE_EVENT(FOnBlockSelected, OnBlockSelected)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	FEditorBuildMenu Menu = FEditorBuildMenu();
	
	FOnMenuSelected OnMenuSelected;
	FOnBlockSelected OnBlockSelected;

	TSharedPtr<SEditorBuildMenu> EditorBuildMenu;

public:

	void OnMenuSlotSelected(int8 Slot);
	void OnExitBuildMode();
};
