#include "EditorBuildMenuOverlay.h"

#include "EditorBuildMenuItem.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorBuildMenuOverlay::Construct(const FArguments& InArgs)
{
	Menu = InArgs._Menu;

	// Building items
	ItemsHorizontalBox = SNew(SHorizontalBox);
	if (ItemsHorizontalBox.IsValid() && Menu.Items.Num() > 0)
	{
		MenuItems.SetNum(Menu.Items.Num());

		for (int32 i = 0; i < Menu.Items.Num(); ++i)
		{
			ItemsHorizontalBox->AddSlot()
			                  .Padding(10.f)
			[
				SAssignNew(MenuItems[i], SEditorBuildMenuItem)
				.Slot(i + 1)
				.Name(Menu.Items[i].Name)
			];
		}
	}

	ChildSlot[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Center)
		.Padding(20.f)
		[

			SAssignNew(ContainerVerticalBox, SVerticalBox)

			// Submenu, opens above, towards up
			// will added in OnMenuSlotSelected()

			// Current menu items
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				[
					SNew(SBorder)
					.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Black"))
					[
						ItemsHorizontalBox.ToSharedRef()
					]
				]
			]

		]
	];
}

void SEditorBuildMenuOverlay::OnMenuSlotSelected(int8 Slot)
{

	// If there is a submenu, it means it is active
	// Route selection to it
	if (SubMenu.IsValid())
	{
		SubMenu->OnMenuSlotSelected(Slot);
		return;
	}
	
	const int8 Index = Slot - 1;

	if (Menu.Items.IsValidIndex(Index) && MenuItems[Index].IsValid())
	{
		MenuItems[Index]->SetSelected();

		if (Menu.Items[Index].Submenu)
		{
			// It's a submenu
			FEditorBuildMenu NewMenu = FEditorBuildMenu(Menu.Items[Index].Submenu);
			if (ContainerVerticalBox.IsValid())
			{
				ContainerVerticalBox->InsertSlot(0)
				                    .AutoHeight()
				[
					SAssignNew(SubMenu, SEditorBuildMenuOverlay)
					.Menu(NewMenu)
				];
			}
		}
		// todo: block?
	}
}
