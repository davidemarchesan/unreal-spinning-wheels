#include "EditorBuildMenuOverlay.h"

#include "EditorBuildMenu.h"
#include "EditorBuildMenuItem.h"
#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorBuildMenuOverlay::Construct(const FArguments& InArgs)
{
	Menu = InArgs._Menu;
	
	ChildSlot[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Fill)
		.Padding(20.f)
		[
			SAssignNew(EditorBuildMenu, SEditorBuildMenu)
			.Menu(Menu)
		]
	];
}

void SEditorBuildMenuOverlay::OnMenuSlotSelected(int8 Slot)
{
	if (EditorBuildMenu.IsValid())
	{
		EditorBuildMenu->OnMenuSlotSelected(Slot);
	}
}

void SEditorBuildMenuOverlay::OnExitBuildMode()
{
	if (EditorBuildMenu.IsValid())
	{
		EditorBuildMenu->CollapseChildren();
	}
}


