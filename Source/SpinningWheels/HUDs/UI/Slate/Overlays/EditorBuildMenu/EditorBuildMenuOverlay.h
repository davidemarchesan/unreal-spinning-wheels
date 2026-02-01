#pragma once
#include "EditorBuildMenuItem.h"
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

	TSharedPtr<SVerticalBox> ContainerVerticalBox;
	TSharedPtr<SHorizontalBox> ItemsHorizontalBox;
	TArray<TSharedPtr<SEditorBuildMenuItem>> MenuItems;
	TSharedPtr<SEditorBuildMenuOverlay> SubMenu;

public:

	void OnMenuSlotSelected(int8 Slot);
	
};
