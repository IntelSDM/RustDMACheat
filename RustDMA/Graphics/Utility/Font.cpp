#include "pch.h"
#include "Font.h"

std::map<std::string, FontInformation> Fonts;
std::unordered_map<std::wstring, IDWriteTextLayout*> TextLayouts;

void CreateFonts(std::string customfontname, std::wstring_view fontname, float size, DWRITE_FONT_WEIGHT weight)
{
	IDWriteTextFormat* text_format;

	std::string fontnamestr(fontname.begin(), fontname.end());
	if (Fonts.find(fontnamestr) == Fonts.end())
	{
		HRESULT result = FontFactory->CreateTextFormat(
			fontname.data(),
			nullptr,
			weight,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			size,
			L"",
			&text_format
		);

		if (SUCCEEDED(result))
		{
			Fonts[customfontname].size = size;
			Fonts[customfontname].font = text_format;
		}
	}
}

Vector2 GetTextSize(std::wstring text, std::string font, size_t font_size)
{
	IDWriteTextLayout* layout;

	if (text.empty())
		return Vector2(0, 0);
	//If font size is 0, use default font size
	if (font_size == 0)
		font_size = Fonts[font].size;
	//Caching text layouts for performance improvements
	if (TextLayouts.find(text) == TextLayouts.end())
	{
		HRESULT hr = FontFactory->CreateTextLayout(
			text.data(),
			static_cast<UINT32>(text.length()),
			Fonts[font].font,
			4096,
			4096,
			&layout
		);

		if (SUCCEEDED(hr))
			TextLayouts[text] = layout;
		else
			return Vector2(0, 0);
	}
	else
		layout = TextLayouts[text];

	DWRITE_TEXT_RANGE range = DWRITE_TEXT_RANGE();
	range.length = text.length();
	range.startPosition = 0;
	layout->SetFontSize(font_size, range);
	DWRITE_TEXT_METRICS metrics;
	if (SUCCEEDED(layout->GetMetrics(&metrics)))
		return Vector2(metrics.widthIncludingTrailingWhitespace, metrics.height);
	return Vector2(0, 0);
}

IDWriteTextFormat* GetFont(std::string fontname)
{
	return Fonts[fontname].font;
}

size_t GetFontSize(std::string fontname)
{
	return Fonts[fontname].size;
}
