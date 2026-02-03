#pragma once

#include "EditorBuildMenuItem.h"
#include "SpinningWheels/Core/EditorBuildMenu.h"

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnMenuSelected, UEditorBuildMenuDataAsset* Menu);
DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnBlockSelected, const int8 Slot);

class SEditorBuildMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditorBuildMenu)
		{
		}
		SLATE_ARGUMENT(EVisibility, Visibility)
		SLATE_ARGUMENT(FEditorBuildMenu, Menu)

		SLATE_EVENT(FOnMenuSelected, OnMenuSelected)
		SLATE_EVENT(FOnBlockSelected, OnBlockSelected)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	FEditorBuildMenu Menu = FEditorBuildMenu();
	
	TSharedPtr<SVerticalBox> ContainerVerticalBox;
	TSharedPtr<SHorizontalBox> ItemsHorizontalBox;
	TArray<TSharedPtr<SEditorBuildMenuItem>> MenuItems;
	int32 ActiveSubMenuIndex = -1;
	TSharedPtr<SWidgetSwitcher> SubMenuSwitcher;

	TMap<int32, TSharedPtr<SEditorBuildMenu>> SubMenus;

	FOnMenuSelected OnMenuSelected;
	FOnBlockSelected OnBlockSelected;

	void NotifyControllerOfNewSubmenu();
	void NotifyControllerOfNewBlock(const int8 Slot);

public:
	void OnMenuSlotSelected(int8 Slot);
	FReply OnMenuSlotClicked(int8 Slot);

	void CollapseChildren();
};
