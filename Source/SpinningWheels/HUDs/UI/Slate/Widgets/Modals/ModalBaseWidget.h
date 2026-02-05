#pragma once

class SModalBase : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModalBase)
		{
		}

		SLATE_NAMED_SLOT(FArguments, HeaderSlot)
		SLATE_NAMED_SLOT(FArguments, BodySlot)
		SLATE_NAMED_SLOT(FArguments, FooterSlot)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SBox> MainBox;
};
