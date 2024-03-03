#pragma once
#include "Entity.h"
#include "Drawing.h"
#include "Input.h"

// due to using templates in c++ classes it is illegal to use a c++ file so this will look rather odd to the other code.
template <typename T>
class Slider : public Entity
{
protected:
	T MinValue;
	T MaxValue;
	T* Value;
	T OutputValue;

	bool Held = false;

	std::wstring Measurement;
	std::wstring OutputString;

	void ConvertValueToString()
	{
		OutputString = std::to_wstring(OutputValue);
		size_t dotpos = OutputString.find(L".");
		if (dotpos != std::string::npos)
		{
			if (OutputString.length() > dotpos + 2)
			{
				OutputString.resize(dotpos + 3); // Truncate to two places after the decimal place
			}
		}
	}

public:
	void Update()
	{
		if (!Parent)
			SetVisible(false);
		if (!IsVisible())
			return;

		ParentPos = Parent->GetParentPos();

		if (!IsKeyDown(VK_LBUTTON))
		{
			if (Held == true)
				ValueChangeEvent();
			Held = false;
		}
		float ratio = (float)(*Value - (float)MinValue) / float(MaxValue - MinValue);
		if ((IsMouseInRectangle(ParentPos.x + Pos.x - 2, ParentPos.y + (Pos.y + 15) - 2, Size.x + 3, Size.y + 3) || IsMouseInRectangle(ParentPos.x + Pos.x + (Size.x * ratio), ParentPos.y + (Pos.y + 15) - (Size.y / 2.2), 5, Size.y * 2.2)) && IsKeyClicked(VK_LBUTTON) && !Blocked)
		{
			Held = true;
		}
		if (Held)
		{
			// make sure no clicks go through when holding.
			const float clamp = std::clamp<float>((float)MousePos.x - (float)(Pos.x + ParentPos.x), 0.00f, (float)Size.x);
			const float ratio = clamp / Size.x;
			*Value = MinValue + (MaxValue - MinValue) * ratio;
			OutputValue = *Value;
			ConvertValueToString();
		}
	}

	void Draw()
	{
		if (!IsVisible())
			return;
		if (!Parent)
			SetVisible(false);

		MyColour rectColour = MenuColours[LIT("Slider")];
		MyColour textColour = MenuColours[LIT("Text")];
		MyColour insideColour = MenuColours[LIT("SliderInside")];
		MyColour outline = MenuColours[LIT("SliderOutline")];
		MyColour widget = MenuColours[LIT("SliderWidget")];
		DrawText(ParentPos.x + Pos.x + Size.x, (ParentPos.y + Pos.y) - 3, OutputString + Measurement, LIT("Verdana"), 11, textColour, Left);
		DrawText(ParentPos.x + Pos.x, (ParentPos.y + Pos.y) - 3, Name + LIT(L": "), LIT("Verdana"), 11, textColour, None);
		OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + (Pos.y + 15), Size.x, Size.y, 1, outline);
		FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + (Pos.y + 15), Size.x, Size.y, Colour(80, 80, 80, 255));
		float ratio = (float)(*Value - (float)MinValue) / float(MaxValue - MinValue);
		FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + (Pos.y + 15), (int)Size.x * ratio, Size.y, insideColour);

		OutlineRoundedRectangle(ParentPos.x + Pos.x + (Size.x * ratio), ParentPos.y + (Pos.y + 15) - (Size.y / 2.2), 5, Size.y * 2.2, 2, 2, outline);
		FilledRoundedRectangle(ParentPos.x + Pos.x + (Size.x * ratio), ParentPos.y + (Pos.y + 15) - (Size.y / 2.2), 5, Size.y * 2.2, 2, widget);
	}

	Slider(int x, int y,int width, std::wstring name, std::wstring measurement, T minvalue, T maxvalue, T* value) : Measurement(measurement), MaxValue(maxvalue), MinValue(minvalue), Value(value)
	{
		Pos = {(float)x, (float)y};
		Name = name;
		Size = { (float)width,5 };
		OutputValue = *Value;
		ConvertValueToString();
		SetVisible(true);
	}
};
