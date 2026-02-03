#include "EditorBuildMenuOverlay.h"

#include "EditorBuildMenu.h"

void SEditorBuildMenuOverlay::Construct(const FArguments& InArgs)
{
	Menu = InArgs._Menu;
	OnMenuSelected = InArgs._OnMenuSelected;
	OnBlockSelected = InArgs._OnBlockSelected;

	ChildSlot[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Fill)
		.Padding(20.f)
		[
			SAssignNew(EditorBuildMenu, SEditorBuildMenu)
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
			})
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
