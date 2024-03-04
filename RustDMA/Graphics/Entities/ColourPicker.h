#pragma once
#include "Entity.h"

class ColourPicker : public Entity
{
protected:
	Vector2 ClickedPos;
	D2D1::ColorF BaseColour = Colour(0, 0, 0, 0);
	D2D1::ColorF* MainColour = &BaseColour;
	D2D1::ColorF* RangeColour = &BaseColour;
	D2D1::ColorF* SelectedColour = &BaseColour;
	Vector2 SelectedPos;

	float Saturation = 255;
	float Brightness = 255;
	bool HeldSaturation = false;

	float Hue = 360;
	bool HeldHue = false;

	bool HeldAlpha = false;
	float Alpha = 255;

	bool ContextActive = false;
	Vector2 ContextPos;
	Vector2 ContextSize;
	void ContextMenu();
	void Copy();
	void Paste();
	std::map<std::wstring, std::function<void()>> ContextNames = {
		{L"Copy", [this]() { Copy(); }},
		{L"Paste", [this]() { Paste(); }}
	};

	bool Open = false;
	static float HueToSliderValue(float hue);
	static float SliderValueToHue(float slidervalue);
	static float AlphaToSliderValue(float alpha);
	static float SaturationToSliderValue(float saturation);

public:
	ColourPicker(float x, float y, D2D1::ColorF* colour);
	void Update();
	void Draw();
};
