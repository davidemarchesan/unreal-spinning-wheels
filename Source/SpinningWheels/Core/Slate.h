#pragma once
#include "Track.h"

DECLARE_DELEGATE_RetVal(FReply, FOnReply)

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnEditTrack, const FTrackSaveData&)

DECLARE_DELEGATE_OneParam(FOnTrackSelected, int32)
DECLARE_DELEGATE_OneParam(FOnTrackUnselected, int32)
