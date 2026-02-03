#pragma once

#include "CoreMinimal.h"

class FMainStyle
{
public:
	static void Initialize();
	static void Shutdown();
	
	static const ISlateStyle& Get();

private:
	static TSharedPtr<FSlateStyleSet> StyleInstance;

	static void InitializeColors();
	static void InitializeFonts();
	// static void InitializeIcons();
	static void InitializeBrushes();
	static void InitializeButtons();
	static void InitializeConstants();
	
};
