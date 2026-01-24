// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/UI/Slate/Styles/MainStyle.h"

class FSpinningWheelsSlateStylesImpl : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		FMainStyle::Initialize();
	}
	virtual void ShutdownModule() override
	{
		FMainStyle::Shutdown();
	}
};
