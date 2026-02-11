#include "PlayPage.h"

#include "SpinningWheels/HUDs/UI/Slate/Pages/PageBase.h"
#include "SpinningWheels/HUDs/UI/Slate/Widgets/CardWidget.h"
#include "Widgets/Layout/SWrapBox.h"

void SPlayPage::Construct(const FArguments& InArgs)
{

	OnPageBack = InArgs._OnPageBack;
	
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
					.Text(FText::FromString("Solo / Host LAN"))
					.OnClicked(InArgs._OnStartLANSession)
				]

				+ SWrapBox::Slot()
				[
					SNew(SCard)
					.Text(FText::FromString("Join LAN"))
					.OnClicked(InArgs._OnJoinLANSession)
				]

				+ SWrapBox::Slot()
				[
					SNew(SCard)
					.Text(FText::FromString("Steam Host"))
					.OnClicked(InArgs._OnStartSteamSession)
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
