#pragma once

class SButtonAction : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SButtonAction) {}

		SLATE_ARGUMENT(FText, ButtonText);
		SLATE_ARGUMENT(FText, ActionText);
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

public:
	
};
