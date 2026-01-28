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

	InitializeColors();
	InitializeFonts();
	// InitializeIcons();
	InitializeBrushes();
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
	check(StyleInstance.IsValid());
	return *StyleInstance;
}

void FMainStyle::InitializeColors()
{
	// https://coolors.co/d8dbe2-a9bcd0-58a4b0-373f51-daa49a
	const FColor AlabasterGrey = FColor::FromHex("D8DBE2");
	const FColor PowderBlue = FColor::FromHex("A9BCD0");
	const FColor PacificBlue = FColor::FromHex("58A4B0");
	const FColor CharcoalBlue = FColor::FromHex("373F51");
	const FColor PowderBlush = FColor::FromHex("DAA49A");
	
	StyleInstance->Set("Color.AlabasterGrey", FLinearColor(AlabasterGrey));
	StyleInstance->Set("Color.PowderBlue", FLinearColor(PowderBlue));
	StyleInstance->Set("Color.PacificBlue", FLinearColor(PacificBlue));
	StyleInstance->Set("Color.CharcoalBlue", FLinearColor(CharcoalBlue));
	StyleInstance->Set("Color.PowderBlush", FLinearColor(PowderBlush));
	
	StyleInstance->Set("Color.Primary.Light", FLinearColor(FColor::FromHex("F5F5F5")));
	StyleInstance->Set("Color.Primary.Dark", FLinearColor(FColor::FromHex("333333")));
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
		FStyleFontData(
			"JetBrains.Regular",
			"/Game/Assets/Fonts/JetBrainsMono/JetBrainsMono-Regular_Font.JetBrainsMono-Regular_Font"
		),
		FStyleFontData(
			"JetBrains.Bold",
			"/Game/Assets/Fonts/JetBrainsMono/JetBrainsMono-Bold_Font.JetBrainsMono-Bold_Font"
		),
	};

	const TMap<FString, float> FontSizes = {
		{"help", 10.f},
		{"p", 14.f},
		{"h1", 29.f},
		{"h2", 25.f},
		{"h3", 22.f},
		{"h4", 18.f},
	};

	for (const FStyleFontData& StyleFont : StyleFonts)
	{
		for (const TPair<FString, float>& FontSize : FontSizes)
		{
			FString StyleFontName = "Font." + StyleFont.Name + "." + FontSize.Key;
			// Example: FB.Font.Regular.sm or FB.Font.Regular.md

			if (UFont* FontAsset = LoadObject<UFont>(nullptr, *StyleFont.Source))
			{
				StyleInstance->Set(*StyleFontName, FSlateFontInfo(FontAsset, FontSize.Value));
			}
		}
	}
}

void FMainStyle::InitializeBrushes()
{
	StyleInstance->Set("Brush.Background.Light", new FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.AlabasterGrey"), 12.f));
	StyleInstance->Set("Brush.Background.Dark", new FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.CharcoalBlue"), 12.f));
}
