#pragma once

DECLARE_DELEGATE_RetVal(FReply, FOnPageBack);

enum class EMenuPage
{
	MP_None,
	MP_Play,
	MP_Tracks
};
