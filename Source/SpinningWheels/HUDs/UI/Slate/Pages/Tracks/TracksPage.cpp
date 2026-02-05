#include "TracksPage.h"

#include "TrackItemWidget.h"
#include "SpinningWheels/HUDs/UI/Slate/Pages/PageBase.h"
#include "Widgets/Layout/SWrapBox.h"

void STracksPage::Construct(const FArguments& InArgs)
{
	OnPageBack = InArgs._OnPageBack;

	ChildSlot[

		SNew(SPageBase)
		.PageTitleText(FText::FromString("Tracks"))
		.BodySlot()
		[
			SNew(SBox)
			[
				SNew(SBorder) // todo: remove, just for testing
				[

					SAssignNew(WrapBox, SWrapBox)
					.Orientation(Orient_Horizontal)
					.UseAllottedSize(true)
					.InnerSlotPadding(FVector2D(10.f))
					.HAlign(HAlign_Center)

					+ SWrapBox::Slot()
					[
						SAssignNew(DefaultTrackItem, STrackItem)
					]
					+ SWrapBox::Slot()
					[
						SNew(STrackItem)
					]
					+ SWrapBox::Slot()
					[
						SNew(STrackItem)
					]
					+ SWrapBox::Slot()
					[
						SNew(STrackItem)
					]
					+ SWrapBox::Slot()
					[
						SNew(STrackItem)
					]
					+ SWrapBox::Slot()
					[
						SNew(STrackItem)
					]
					+ SWrapBox::Slot()
					[
						SNew(STrackItem)
					]
				]
			]

		]

	];

	if (DefaultTrackItem.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("trying to focus the default item"));
		FSlateApplication::Get().SetKeyboardFocus(DefaultTrackItem);
	}
}

bool STracksPage::SupportsKeyboardFocus() const
{
	return true;
}

FReply STracksPage::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (OnPageBack.IsBound())
		{
			return OnPageBack.Execute();
		}
	}
	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

FReply STracksPage::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Tracks page just received focus"));
	return SCompoundWidget::OnFocusReceived(MyGeometry, InFocusEvent);
}
