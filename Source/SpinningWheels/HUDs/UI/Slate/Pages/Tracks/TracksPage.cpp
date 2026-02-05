#include "TracksPage.h"

#include "SpinningWheels/HUDs/UI/Slate/Pages/PageBase.h"

void STracksPage::Construct(const FArguments& InArgs)
{

	OnPageBack = InArgs._OnPageBack;
	
	ChildSlot[

		SNew(SPageBase)
		.PageTitleText(FText::FromString("Tracks"))

	];
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
