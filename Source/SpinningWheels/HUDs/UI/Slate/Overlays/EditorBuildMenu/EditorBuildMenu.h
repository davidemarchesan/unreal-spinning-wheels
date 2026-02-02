#pragma once

#include "EditorBuildMenuItem.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"

class SEditorBuildMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditorBuildMenu)
		{
		}
		SLATE_ARGUMENT(EVisibility, Visibility)
		SLATE_ARGUMENT(FEditorBuildMenu, Menu)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	FEditorBuildMenu Menu = FEditorBuildMenu();
	
	TSharedPtr<SVerticalBox> ContainerVerticalBox;
	TSharedPtr<SHorizontalBox> ItemsHorizontalBox;
	TArray<TSharedPtr<SEditorBuildMenuItem>> MenuItems;
	TSharedPtr<SWidgetSwitcher> SubMenuSwitcher;
	// TSharedPtr<SEditorBuildMenu> SubMenu;

	TMap<int32, TSharedPtr<SEditorBuildMenu>> SubMenus;

	FButtonStyle ButtonStyle; // todo: put it on main style

public:
	void OnMenuSlotSelected(int8 Slot);

	void CollapseChildren();
	void Collapse();
};
