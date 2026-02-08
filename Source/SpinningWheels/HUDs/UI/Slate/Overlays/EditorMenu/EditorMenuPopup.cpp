#include "EditorMenuPopup.h"

#include "SpinningWheels/HUDs/UI/Slate/Styles/MainStyle.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/MenuItemWidget.h"

void SEditorMenuPopup::Construct(const FArguments& InArgs)
{

	OnBack = InArgs._OnBack;
	
	ChildSlot[
		SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.MinDesiredWidth(700.f)
			[
				SNew(SBorder)
				.BorderImage(FMainStyle::Get().GetBrush("Brush.Background.Dark"))
				.Padding(FMainStyle::Get().GetMargin("Padding.Box"))
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SMenuItem)
						.Text(FText::FromString("Continue"))
						.OnClicked(this, &SEditorMenuPopup::ExecuteOnBack)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SMenuItem)
						.Text(FText::FromString("Test track"))
						.OnClicked(InArgs._OnTestTrack)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SMenuItem)
						.Text(FText::FromString("Save track"))
						.OnClicked(InArgs._OnSaveTrack)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SMenuItem)
						.Text(FText::FromString("Return to editor"))
						.OnClicked(InArgs._OnReturnToEditor)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SSpacer)
						.Size(FVector2D(1.f, 20.f))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SMenuItem)
						.Text(FText::FromString("Exit to main menu"))
						.OnClicked(InArgs._OnGoToMainMenu)
					]
				]

			]
		]
	];
}

FReply SEditorMenuPopup::ExecuteOnBack()
{
	if (OnBack.IsBound())
	{
		return OnBack.Execute();
	}
	return FReply::Unhandled();
}
