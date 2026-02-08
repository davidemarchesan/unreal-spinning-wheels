#pragma once
#include "SpinningWheels/Core/Slate.h"

class SPlayMainOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayMainOverlay)
	{
	}

	SLATE_EVENT(FOnReply, OnHost)
	SLATE_EVENT(FOnReply, OnJoin)

SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:


public:
};
