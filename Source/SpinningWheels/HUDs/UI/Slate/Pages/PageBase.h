#pragma once

class SPageBase : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPageBase)
		{
		}

		SLATE_ARGUMENT(FText, PageTitleText)

		SLATE_NAMED_SLOT(FArguments, BodySlot)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SBox> MainBox;
};
