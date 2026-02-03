#include "EditorBuildMenu.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SEditorBuildMenu::Construct(const FArguments& InArgs)
{
	EVisibility Visibility = InArgs._Visibility;
	Menu = InArgs._Menu;

	OnMenuSelected = InArgs._OnMenuSelected;
	OnBlockSelected = InArgs._OnBlockSelected;

	// Building items and submenus
	ItemsHorizontalBox = SNew(SHorizontalBox);
	SubMenuSwitcher = SNew(SWidgetSwitcher).Visibility(EVisibility::Collapsed);

	if (ItemsHorizontalBox.IsValid() && SubMenuSwitcher.IsValid() && Menu.Items.Num() > 0)
	{
		MenuItems.SetNum(Menu.Items.Num());

		for (int32 i = 0; i < Menu.Items.Num(); ++i)
		{
			// Adding a slot to horizontal box
			ItemsHorizontalBox->AddSlot()
			                  .Padding(10.f)
			[
				SAssignNew(MenuItems[i], SEditorBuildMenuItem)
				.Slot(i + 1)
				.Name(Menu.Items[i].Name)
				.OnMenuSlotClicked(this, &SEditorBuildMenu::OnMenuSlotClicked)
			];

			// If this item leads to another menu (submenu)
			// prepare it for switcher widget
			if (Menu.Items[i].Submenu)
			{
				TSharedPtr<SEditorBuildMenu> SubMenu = SNew(SEditorBuildMenu)
					.Menu(Menu.Items[i].Submenu)
					.OnMenuSelected_Lambda([this](UEditorBuildMenuDataAsset* Menu)
					{
						if (this && Menu && OnMenuSelected.IsBound())
						{
							FReply Reply = OnMenuSelected.Execute(Menu);
							return Reply;
						}
						return FReply::Unhandled();
					})
					.OnBlockSelected_Lambda([this](const int8 Slot)
					{
						if (this && OnBlockSelected.IsBound())
						{
							FReply Reply = OnBlockSelected.Execute(Slot);
							return Reply;
						}
						return FReply::Unhandled();
					});

				if (SubMenu.IsValid())
				{
					SubMenus.Add(i, SubMenu);
					SubMenuSwitcher->AddSlot()
					[
						SubMenu.ToSharedRef()
					];
				}
			}
		}
	}

	ChildSlot[
		SAssignNew(ContainerVerticalBox, SVerticalBox)
		.Visibility(Visibility)

		// Submenu, opens above, towards up
		// will added in OnMenuSlotSelected()

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SubMenuSwitcher.ToSharedRef()
		]

		// Current menu items
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SBorder)
				.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Black"))
				[
					ItemsHorizontalBox.ToSharedRef()
				]
			]
		]

	];
}

void SEditorBuildMenu::NotifyControllerOfNewSubmenu()
{
	// Means that during OnMenuSlotSelected() a submenu has been activated
	if (ActiveSubMenuIndex != -1)
	{
		// Notify controller that we opened a new menu
		if (Menu.Items.IsValidIndex(ActiveSubMenuIndex) && OnMenuSelected.IsBound())
		{
			OnMenuSelected.Execute(Menu.Items[ActiveSubMenuIndex].Submenu);
		}
	}
}

void SEditorBuildMenu::NotifyControllerOfNewBlock(const int8 Slot)
{
	if (OnBlockSelected.IsBound())
	{
		FReply Reply = OnBlockSelected.Execute(Slot);
	}
}

void SEditorBuildMenu::OnMenuSlotSelected(int8 Slot)
{
	const int8 Index = Slot - 1;

	// If there is an active submenu
	// route the request to it
	if (ActiveSubMenuIndex != -1)
	{
		if (const TSharedPtr<SEditorBuildMenu>* SubMenuPtr = SubMenus.Find(ActiveSubMenuIndex))
		{
			SubMenuPtr->Get()->OnMenuSlotSelected(Slot);
			return;
		}
	}

	if (Menu.Items.IsValidIndex(Index) && MenuItems[Index].IsValid())
	{
		// Deselect all slots (but this one of course)
		for (int32 i = 0; i < MenuItems.Num(); ++i)
		{
			if (i != Index)
			{
				MenuItems[i]->SetUnselected();
			}
		}

		MenuItems[Index]->SetSelected();

		// This slot has a submenu as child, show it
		if (SubMenuSwitcher.IsValid())
		{
			if (const TSharedPtr<SEditorBuildMenu>* SubMenu = SubMenus.Find(Index))
			{
				SubMenuSwitcher->SetActiveWidget(SubMenu->ToSharedRef());
				SubMenuSwitcher->SetVisibility(EVisibility::Visible);
				ActiveSubMenuIndex = Index;
			}
		}
	}
}

FReply SEditorBuildMenu::OnMenuSlotClicked(int8 Slot)
{
	const int8 Index = Slot - 1;

	if (Menu.Items.IsValidIndex(Index) == false)
	{
		return FReply::Handled();
	}

	if (Menu.Items[Index].Submenu)
	{
		if (ActiveSubMenuIndex == -1)
		{
			// No submenu was open
			OnMenuSlotSelected(Slot);
			NotifyControllerOfNewSubmenu();
			return FReply::Handled();
		}

		if (ActiveSubMenuIndex == Index)
		{
			// Close the current submenu
			CollapseChildren();
			return FReply::Handled();
		}

		// A submenu was open, close it
		// and open the new one
		CollapseChildren();
		OnMenuSlotSelected(Slot);
		NotifyControllerOfNewSubmenu();
	}
	else if (Menu.Items[Index].BlocksTableRow.IsNull() == false)
	{
		NotifyControllerOfNewBlock(Slot);
	}

	return FReply::Handled();
}

void SEditorBuildMenu::CollapseChildren()
{
	for (int32 i = 0; i < MenuItems.Num(); ++i)
	{
		MenuItems[i]->SetUnselected();
	}

	for (TPair<int32, TSharedPtr<SEditorBuildMenu>> Item : SubMenus)
	{
		Item.Value->CollapseChildren();
	}

	SubMenuSwitcher->SetVisibility(EVisibility::Collapsed);
	ActiveSubMenuIndex = -1;
}
