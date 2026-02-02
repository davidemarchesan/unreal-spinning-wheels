#pragma once

#include "EditorBuildMenu.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"

class SEditorBuildMenuOverlay : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SEditorBuildMenuOverlay) {}
		SLATE_ARGUMENT(FEditorBuildMenu, Menu)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	FEditorBuildMenu Menu = FEditorBuildMenu();

	TSharedPtr<SEditorBuildMenu> EditorBuildMenu;

public:

	void OnMenuSlotSelected(int8 Slot);
	void OnExitBuildMode();
	
};
