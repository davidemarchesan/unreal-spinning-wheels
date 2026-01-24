#include "MainStyle.h"

#include "Engine/Font.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FMainStyle::StyleInstance = nullptr;

struct FStyleFontData
{
	FString Name;
	FString Source;
};

void FMainStyle::Initialize()
{
	if (StyleInstance.IsValid())
	{
		return;
	}

	StyleInstance = MakeShareable(new FSlateStyleSet("FacilityBreachStyle"));

	InitializeFonts();
	// InitializeIcons();
	// InitializeBrushes();
	// InitializeButtons();

	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);

	UE_LOG(LogTemp, Warning, TEXT("Slate style is initialized"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Slate style initialize");
	}
}

void FMainStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Slate style shutdown");
		}
	}
}

const ISlateStyle& FMainStyle::Get()
{
	return *StyleInstance;
}

void FMainStyle::InitializeFonts()
{
	const TArray<FStyleFontData> StyleFonts = {
		FStyleFontData(
			"Roboto.Regular",
			"/Game/Assets/Fonts/Roboto/Roboto-Regular_Font.Roboto-Regular_Font"
		),
		FStyleFontData(
			"Roboto.SemiBold",
			"/Game/Assets/Fonts/Roboto/Roboto-SemiBold_Font.Roboto-SemiBold_Font"
		),
		FStyleFontData(
			"Lato.Regular",
			"/Game/Assets/Fonts/Lato/Lato-Regular_Font.Lato-Regular_Font"
		),
		FStyleFontData(
			"Lato.Bold",
			"/Game/Assets/Fonts/Lato/Lato-Bold_Font.Lato-Bold_Font"
		),
	};

	const TMap<FString, float> FontSizes = {
		{"help", 10.f},
		{"p", 14.f},
		{"h1", 24.f},
		{"h2", 22.f},
		{"h3", 20.f},
		{"h4", 18.f},
	};

	for (const FStyleFontData& StyleFont : StyleFonts)
	{
		for (const TPair<FString, float>& FontSize : FontSizes)
		{
			FString StyleFontName = StyleFont.Name + "." + FontSize.Key;
			// Example: FB.Font.Regular.sm or FB.Font.Regular.md

			if (UFont* FontAsset = LoadObject<UFont>(nullptr, *StyleFont.Source))
			{
				StyleInstance->Set(*StyleFontName, FSlateFontInfo(FontAsset, FontSize.Value));
			}
		}
	}
}
