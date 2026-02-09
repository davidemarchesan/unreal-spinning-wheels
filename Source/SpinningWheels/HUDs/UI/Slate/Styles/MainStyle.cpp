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
	InitializeTextStyles();
	InitializeButtonStyles();
	InitializeConstants();

	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
}

void FMainStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();
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
	// const FColor AlabasterGrey = FColor::FromHex("D8DBE2");
	// const FColor PowderBlue = FColor::FromHex("A9BCD0");
	// const FColor PacificBlue = FColor::FromHex("58A4B0");
	// const FColor CharcoalBlue = FColor::FromHex("373F51");
	// const FColor PowderBlush = FColor::FromHex("DAA49A");
	//
	// StyleInstance->Set("Color.AlabasterGrey", FLinearColor(AlabasterGrey));
	// StyleInstance->Set("Color.PowderBlue", FLinearColor(PowderBlue));
	// StyleInstance->Set("Color.PacificBlue", FLinearColor(PacificBlue));
	// StyleInstance->Set("Color.CharcoalBlue", FLinearColor(CharcoalBlue));
	// StyleInstance->Set("Color.PowderBlush", FLinearColor(PowderBlush));
	//
	// StyleInstance->Set("Color.Primary.Light", FLinearColor(FColor::FromHex("F5F5F5")));
	// StyleInstance->Set("Color.Primary.Dark", FLinearColor(FColor::FromHex("333333")));


	StyleInstance->Set("Color.Time.Purple", FLinearColor(FColor::FromHex("B64CFF")));
	StyleInstance->Set("Color.Time.Green", FLinearColor(FColor::FromHex("00E676")));
	StyleInstance->Set("Color.Time.Red", FLinearColor(FColor::FromHex("FF3B30")));

	// New ones
	StyleInstance->Set("Color.Yellow.Primary", FLinearColor(FColor::FromHex("FFE04D")));
	StyleInstance->Set("Color.Yellow.Secondary", FLinearColor(FColor::FromHex("FFD400")));
	StyleInstance->Set("Color.Yellow.Tertiary", FLinearColor(FColor::FromHex("E6BF00")));

	StyleInstance->Set("Color.Black.Primary", FLinearColor(FColor::FromHex("1A1A1A")));
	StyleInstance->Set("Color.Black.Secondary", FLinearColor(FColor::FromHex("0B0B0B")));
	StyleInstance->Set("Color.Black.Tertiary", FLinearColor(FColor::FromHex("2A2A2A")));

	StyleInstance->Set("Color.White.Primary", FLinearColor(FColor::FromHex("F5F5F5")));
	StyleInstance->Set("Color.White.Secondary", FLinearColor(FColor::FromHex("FFFFFF")));
	StyleInstance->Set("Color.White.Tertiary", FLinearColor(FColor::FromHex("EAEAEA")));

	StyleInstance->Set("Color.Text.Light.Primary", FLinearColor(FColor::FromHex("F5F5F5")));
	StyleInstance->Set("Color.Text.Light.Secondary", FLinearColor(FColor::FromHex("BDBDBD")));

	StyleInstance->Set("Color.Text.Dark.Primary", FLinearColor(FColor::FromHex("1A1A1A")));
	StyleInstance->Set("Color.Text.Dark.Secondary", FLinearColor(FColor::FromHex("4A4A4A")));

	StyleInstance->Set("Color.Background.Dark", FLinearColor(FColor::FromHex("141414")));
}

void FMainStyle::InitializeFonts()
{
	const TArray<FStyleFontData> StyleFonts = {
		// Funnel Sans - Standard
		FStyleFontData(
			"Funnel.Regular",
			"/Game/Assets/Fonts/Funnel/FunnelSans-Regular_Font.FunnelSans-Regular_Font"
		),
		FStyleFontData(
			"Funnel.Regular.Italic",
			"/Game/Assets/Fonts/Funnel/FunnelSans-Italic_Font.FunnelSans-Italic_Font"
		),
		FStyleFontData(
			"Funnel.Medium",
			"/Game/Assets/Fonts/Funnel/FunnelSans-Medium_Font.FunnelSans-Medium_Font"
		),
		FStyleFontData(
			"Funnel.Medium.Italic",
			"/Game/Assets/Fonts/Funnel/FunnelSans-MediumItalic_Font.FunnelSans-MediumItalic_Font"
		),
		FStyleFontData(
			"Funnel.SemiBold",
			"/Game/Assets/Fonts/Funnel/FunnelSans-SemiBold_Font.FunnelSans-SemiBold_Font"
		),
		FStyleFontData(
			"Funnel.SemiBold.Italic",
			"/Game/Assets/Fonts/Funnel/FunnelSans-SemiBoldItalic_Font.FunnelSans-SemiBoldItalic_Font"
		),

		// JetBrains - mono
		FStyleFontData(
			"JetBrains.Regular",
			"/Game/Assets/Fonts/JetBrainsMono/JetBrainsMono-Regular_Font.JetBrainsMono-Regular_Font"
		),
		FStyleFontData(
			"JetBrains.Bold",
			"/Game/Assets/Fonts/JetBrainsMono/JetBrainsMono-Bold_Font.JetBrainsMono-Bold_Font"
		)
	};

	const TMap<FString, float> FontSizes = {
		{"help", 10.f},
		{"p", 12.f},
		{"h1", 29.f},
		{"h2", 24.f},
		{"h3", 20.f},
		{"h4", 16.f},
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
	StyleInstance->Set("Brush.Background.Light",
	                   new FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.White.Primary"), 12.f));
	StyleInstance->Set("Brush.Background.Dark",
	                   new FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Background.Dark"), 12.f));

	StyleInstance->Set("Brush.ButtonAction.Button",
	                   new FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.White.Primary"), 6.f));


	StyleInstance->Set("Brush.Card.Normal",
	                   new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 12.f,
	                                             FLinearColor::Transparent, 4.f));
	StyleInstance->Set("Brush.Card.Hovered",
	                   new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 12.f,
	                                             FMainStyle::Get().GetColor("Color.Text.Light.Primary"), 4.f));

	StyleInstance->Set("Brush.TrackCard.Normal",
	                   new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Secondary"), 12.f,
	                                             FLinearColor::Transparent, 4.f));
	StyleInstance->Set("Brush.TrackCard.Hovered",
	                   new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Primary"), 12.f,
	                                             FLinearColor::Transparent, 4.f));
	StyleInstance->Set("Brush.TrackCard.Selected",
	                   new FSlateRoundedBoxBrush(FMainStyle::Get().GetColor("Color.Black.Primary"), 12.f,
	                                             FMainStyle::Get().GetColor("Color.Text.Light.Primary"), 4.f));
}

void FMainStyle::InitializeTextStyles()
{
	// Normal text styles
	FTextBlockStyle GameTitleTextStyle = FTextBlockStyle();
	GameTitleTextStyle.SetColorAndOpacity(StyleInstance->GetColor("Color.Yellow.Primary"));
	GameTitleTextStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.SemiBold.Italic.h1"));
	GameTitleTextStyle.SetTransformPolicy(ETextTransformPolicy::ToUpper);
	StyleInstance->Set("Text.Game.Title", GameTitleTextStyle);

	FTextBlockStyle PageTitleTextStyle = FTextBlockStyle();
	PageTitleTextStyle.SetColorAndOpacity(StyleInstance->GetColor("Color.Yellow.Primary"));
	PageTitleTextStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.Regular.Italic.h2"));
	StyleInstance->Set("Text.Page.Title", PageTitleTextStyle);

	FTextBlockStyle BoxTitleTextStyle = FTextBlockStyle();
	BoxTitleTextStyle.SetColorAndOpacity(StyleInstance->GetColor("Color.Yellow.Primary"));
	BoxTitleTextStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.Regular.Italic.h4"));
	StyleInstance->Set("Text.Box.Title", BoxTitleTextStyle);

	FTextBlockStyle PTextStyle = FTextBlockStyle();
	PTextStyle.SetColorAndOpacity(StyleInstance->GetColor("Color.Text.Light.Primary"));
	PTextStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.Regular.p"));
	StyleInstance->Set("Text.P", PTextStyle);

	FTextBlockStyle ButtonActionButtonTextStyle = FTextBlockStyle();
	ButtonActionButtonTextStyle.SetColorAndOpacity(FMainStyle::Get().GetColor("Color.Black.Secondary"));
	ButtonActionButtonTextStyle.SetFont(FMainStyle::Get().GetFontStyle("Font.Funnel.SemiBold.Italic.p"));
	ButtonActionButtonTextStyle.SetTransformPolicy(ETextTransformPolicy::ToUpper);
	StyleInstance->Set("Text.ButtonAction.Button", ButtonActionButtonTextStyle);

	FTextBlockStyle ButtonActionActionTextStyle = FTextBlockStyle();
	ButtonActionActionTextStyle.SetColorAndOpacity(FMainStyle::Get().GetColor("Color.White.Primary"));
	ButtonActionActionTextStyle.SetFont(FMainStyle::Get().GetFontStyle("Font.Funnel.SemiBold.p"));
	ButtonActionActionTextStyle.SetTransformPolicy(ETextTransformPolicy::ToUpper);
	StyleInstance->Set("Text.ButtonAction.Action", ButtonActionActionTextStyle);

	// Button text styles
	FTextBlockStyle ButtonPrimaryLargeTextStyle = FTextBlockStyle();
	ButtonPrimaryLargeTextStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.SemiBold.Italic.h4"));
	ButtonPrimaryLargeTextStyle.SetTransformPolicy(ETextTransformPolicy::ToUpper);
	ButtonPrimaryLargeTextStyle.SetColorAndOpacity(StyleInstance->GetColor("Color.Black.Secondary"));
	StyleInstance->Set("Text.Button.Primary.Large", ButtonPrimaryLargeTextStyle);

	FTextBlockStyle ButtonPrimaryTextStyle = FTextBlockStyle();
	ButtonPrimaryTextStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.SemiBold.Italic.p"));
	ButtonPrimaryTextStyle.SetColorAndOpacity(StyleInstance->GetColor("Color.Black.Secondary"));
	ButtonPrimaryTextStyle.SetTransformPolicy(ETextTransformPolicy::ToUpper);
	StyleInstance->Set("Text.Button.Primary", ButtonPrimaryTextStyle);

	FTextBlockStyle ButtonSecondaryTextStyle = FTextBlockStyle();
	ButtonSecondaryTextStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.SemiBold.Italic.p"));
	ButtonSecondaryTextStyle.SetColorAndOpacity(StyleInstance->GetColor("Color.Text.Light.Primary"));
	ButtonSecondaryTextStyle.SetTransformPolicy(ETextTransformPolicy::ToUpper);
	StyleInstance->Set("Text.Button.Secondary", ButtonSecondaryTextStyle);

	// Editable text styles
	FEditableTextBoxStyle EditableTextBoxStyle = FEditableTextBoxStyle();
	EditableTextBoxStyle.SetFont(StyleInstance->GetFontStyle("Font.Funnel.Regular.p"));
	EditableTextBoxStyle.SetBackgroundImageNormal(FSlateRoundedBoxBrush(
		StyleInstance->GetColor("Color.Text.Dark.Primary"),
		6.f,
		FLinearColor::Transparent,
		1.f
	));
	EditableTextBoxStyle.SetBackgroundImageHovered(FSlateRoundedBoxBrush(
		StyleInstance->GetColor("Color.Text.Dark.Primary"),
		6.f,
		StyleInstance->GetColor("Color.Text.Dark.Secondary"),
		1.f
	));
	EditableTextBoxStyle.SetBackgroundImageFocused(FSlateRoundedBoxBrush(
		StyleInstance->GetColor("Color.Text.Dark.Primary"),
		6.f,
		StyleInstance->GetColor("Color.Yellow.Primary"),
		1.f
	));
	EditableTextBoxStyle.SetPadding(FMargin(7.f, 5));
	EditableTextBoxStyle.SetForegroundColor(StyleInstance->GetColor("Color.Text.Light.Primary"));
	EditableTextBoxStyle.SetFocusedForegroundColor(StyleInstance->GetColor("Color.Text.Light.Primary"));
	StyleInstance->Set("Text.Editable", EditableTextBoxStyle);
}

void FMainStyle::InitializeButtonStyles()
{
	FButtonStyle ButtonPrimaryLargeStyle = FButtonStyle();
	ButtonPrimaryLargeStyle.SetNormal(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Yellow.Secondary"), 12.f));
	ButtonPrimaryLargeStyle.SetHovered(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Yellow.Primary"), 12.f));
	ButtonPrimaryLargeStyle.SetPressed(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Yellow.Tertiary"), 12.f));
	ButtonPrimaryLargeStyle.SetNormalPadding(FMargin(15.f, 5.f));
	ButtonPrimaryLargeStyle.SetPressedPadding(FMargin(15.f, 5.f));
	StyleInstance->Set("Button.Primary.Large", ButtonPrimaryLargeStyle);

	FButtonStyle ButtonPrimaryStyle = FButtonStyle();
	ButtonPrimaryStyle.SetNormal(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Yellow.Secondary"), 6.f));
	ButtonPrimaryStyle.SetHovered(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Yellow.Primary"), 6.f));
	ButtonPrimaryStyle.SetPressed(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Yellow.Tertiary"), 6.f));
	ButtonPrimaryStyle.SetNormalPadding(FMargin(7.f, 3.f));
	ButtonPrimaryStyle.SetPressedPadding(FMargin(7.f, 3.f));
	StyleInstance->Set("Button.Primary", ButtonPrimaryStyle);

	FButtonStyle ButtonSecondaryStyle = FButtonStyle();
	ButtonSecondaryStyle.SetNormal(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Secondary"), 6.f));
	ButtonSecondaryStyle.SetHovered(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Primary"), 6.f));
	ButtonSecondaryStyle.SetPressed(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Tertiary"), 6.f));
	ButtonSecondaryStyle.SetNormalPadding(FMargin(7.f, 3.f));
	ButtonSecondaryStyle.SetPressedPadding(FMargin(7.f, 3.f));
	StyleInstance->Set("Button.Secondary", ButtonSecondaryStyle);

	FButtonStyle ButtonMenuBuildItemStyle = FButtonStyle();
	ButtonMenuBuildItemStyle.SetNormal(FSlateRoundedBoxBrush(FLinearColor::Transparent, 6.f));
	ButtonMenuBuildItemStyle.SetHovered(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Primary"), 6.f));
	ButtonMenuBuildItemStyle.SetPressed(FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Tertiary"), 6.f));
	StyleInstance->Set("Button.Menu.Build.Item", ButtonMenuBuildItemStyle);

	FButtonStyle ButtonMenuBuildItemSelectedStyle = FButtonStyle();
	ButtonMenuBuildItemSelectedStyle.SetNormal(
		FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Primary"), 6.f));
	ButtonMenuBuildItemSelectedStyle.SetHovered(
		FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Primary"), 6.f));
	ButtonMenuBuildItemSelectedStyle.SetPressed(
		FSlateRoundedBoxBrush(StyleInstance->GetColor("Color.Black.Tertiary"), 6.f));
	StyleInstance->Set("Button.Menu.Build.Item.Selected", ButtonMenuBuildItemSelectedStyle);
}

void FMainStyle::InitializeConstants()
{
	// Paddings
	StyleInstance->Set("Padding.SafeArea", FMargin(40.f));
	StyleInstance->Set("Padding.Box", FMargin(15.f));
}
