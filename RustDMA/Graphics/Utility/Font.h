#pragma once
#include "Vector.h"

enum FontAlignment
{
	Left = 0,
	Centre = 1,
	Right = 2,
	CentreLeft = 3,
	CentreRight = 4,
	CentreCentre = 5,
	None = 6
};

extern void CreateFonts(std::string customfontname, std::wstring_view fontname, float size, DWRITE_FONT_WEIGHT weight);
extern Vector2 GetTextSize(std::wstring text, std::string font, size_t font_size = 0);
extern size_t GetFontSize(std::string fontname);
extern IDWriteTextFormat* GetFont(std::string fontname);
