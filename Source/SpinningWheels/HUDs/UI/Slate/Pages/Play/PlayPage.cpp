#include "PlayPage.h"

#include "SpinningWheels/HUDs/UI/Slate/Pages/PageBase.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/CardWidget.h"
#include "Widgets/Layout/SWrapBox.h"

void SPlayPage::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SNew(SPageBase)
		.BodySlot()
		[
			SNew(SBox)
			.VAlign(VAlign_Center)
			[
				SNew(SWrapBox)
				.Orientation(Orient_Horizontal)
				.UseAllottedSize(true)
				.InnerSlotPadding(FVector2D(10.f))
				.HAlign(HAlign_Center)

				+ SWrapBox::Slot()
				[
					SNew(SCard)
					.Text(FText::FromString("Host / Solo"))
					.OnClicked(InArgs._OnHost)

				]

				+ SWrapBox::Slot()
				[
					SNew(SCard)
					.Text(FText::FromString("Join"))
					.OnClicked(InArgs._OnJoin)
				]
			]
		]
	];
}

bool SPlayPage::SupportsKeyboardFocus() const
{
	return true;
}

FReply SPlayPage::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (OnPageBack.IsBound())
	{
		return OnPageBack.Execute();
	}

	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}
