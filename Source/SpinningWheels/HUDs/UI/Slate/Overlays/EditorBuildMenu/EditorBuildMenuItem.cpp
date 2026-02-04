#include "EditorBuildMenuItem.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"

void SEditorBuildMenuItem::Construct(const FArguments& InArgs)
{
	Slot = InArgs._Slot;
	const FText Name = InArgs._Name;

	OnMenuSlotClicked = InArgs._OnMenuSlotClicked;
	
	ChildSlot[

		SNew(SBox)
		.HeightOverride(50.f)
		.WidthOverride(50.f)
		[
			SAssignNew(Button, SButton)
			.ButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Menu.Build.Item"))
			.OnClicked(this, &SEditorBuildMenuItem::OnClicked)
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.Regular.p"))
					.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
					.Justification(ETextJustify::Center)
					.Text(FText::AsNumber(Slot))
				]

				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(FMainStyle::Get().GetFontStyle("Font.Funnel.Regular.p"))
					.ColorAndOpacity(FMainStyle::Get().GetColor("Color.Text.Light.Primary"))
					.Justification(ETextJustify::Center)
					.Text(Name)
				]
			]
		]

	];
}

FReply SEditorBuildMenuItem::OnClicked()
{
	if (OnMenuSlotClicked.IsBound())
	{
		FReply Reply = OnMenuSlotClicked.Execute(Slot);
		return Reply;
	}
	return FReply::Unhandled();
}

void SEditorBuildMenuItem::SetSelected()
{
	if (Button)
	{
		Button->SetButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Menu.Build.Item.Selected"));
	}
}

void SEditorBuildMenuItem::SetUnselected()
{
	if (Button)
	{
		Button->SetButtonStyle(&FMainStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Menu.Build.Item"));
	}
}
