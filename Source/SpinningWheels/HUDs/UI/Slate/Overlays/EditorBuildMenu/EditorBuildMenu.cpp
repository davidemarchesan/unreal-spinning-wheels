#include "EditorBuildMenu.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SEditorBuildMenu::Construct(const FArguments& InArgs)
{
	EVisibility Visibility = InArgs._Visibility;
	Menu = InArgs._Menu;

	// Building items and submenus
	ItemsHorizontalBox = SNew(SHorizontalBox);
	SubMenuSwitcher = SNew(SWidgetSwitcher);

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
			];

			// If this item leads to another menu (submenu)
			// prepare it for switcher widget
			if (Menu.Items[i].Submenu)
			{
				TSharedPtr<SEditorBuildMenu> SubMenu = SNew(SEditorBuildMenu)
					.Menu(Menu.Items[i].Submenu);

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

	// SubMenuSwitcher->SetActiveWidgetIndex(INDEX_NONE);

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

void SEditorBuildMenu::OnMenuSlotSelected(int8 Slot)
{
	const int8 Index = Slot - 1;

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

		if (SubMenus[Index].IsValid() && SubMenuSwitcher.IsValid())
		{
			SubMenuSwitcher->SetActiveWidget(SubMenus[Index].ToSharedRef());
		}
		
		// if (Menu.Items[Index].Submenu)
		// {
		// 	// It's a submenu
		// 	FEditorBuildMenu NewMenu = FEditorBuildMenu(Menu.Items[Index].Submenu);
		// 	if (ContainerVerticalBox.IsValid())
		// 	{
		// 		ContainerVerticalBox->InsertSlot(0)
		// 		                    .AutoHeight()
		// 		[
		// 			SAssignNew(SubMenu, SEditorBuildMenu)
		// 			.Menu(NewMenu)
		// 		];
		// 	}
		// }
	}
}

void SEditorBuildMenu::CollapseChildren()
{
	// if (SubMenu.IsValid())
	// {
	// 	SubMenu->Collapse();
	// }
	//
	// for (int32 i = 0; i < MenuItems.Num(); ++i)
	// {
	// 	MenuItems[i]->SetUnselected();
	// }
}

void SEditorBuildMenu::Collapse()
{
	// if (SubMenu.IsValid())
	// {
	// 	SubMenu->Collapse();
	// }
	//
	// for (int32 i = 0; i < MenuItems.Num(); ++i)
	// {
	// 	MenuItems[i]->SetUnselected();
	// }
	//
	// if (ContainerVerticalBox.IsValid())
	// {
	// 	ContainerVerticalBox->SetVisibility(EVisibility::Collapsed);
	// }
}
