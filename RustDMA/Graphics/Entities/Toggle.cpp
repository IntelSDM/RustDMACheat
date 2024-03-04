#include "pch.h"
#include "Toggle.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"

Toggle::Toggle(float x, float y, std::wstring text, bool* data = nullptr)
{
	Toggle::Pos = {x, y};
	Toggle::Size = {11, 11};
	Toggle::Name = text;
	Toggle::TextSize = GetTextSize(text, LIT("Verdana"), 12);
	Data = data;
	SetVisible(true);
}

void Toggle::Update()
{
	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;
	auto currenttime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltatime = currenttime - Toggle::LastUpdateTime;
	Toggle::LastUpdateTime = currenttime;
	double deltaseconds = deltatime.count();
	Toggle::ParentPos = Toggle::Parent->GetParentPos();
	if (!Toggle::Blocked)
	{
		// square click check
		if (IsMouseInRectangle(Toggle::Pos + Toggle::ParentPos, Toggle::Size) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClick < (clock() * 0.00001f))
		{
			*Toggle::Data = !(*Toggle::Data);
			Toggle::LastClick = (clock() * 0.00001f) + 0.002f;
			Toggle::ValueChangeEvent();
		}
		// text click check
		Vector2 textdimensions = {Toggle::TextSize.x + 4, Toggle::TextSize.y};
		if (IsMouseInRectangle(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x), Toggle::ParentPos.y + Toggle::Pos.y - 1, Toggle::TextSize.x, Toggle::TextSize.y) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClick < (clock() * 0.00001f))
		{
			*Toggle::Data = !(*Toggle::Data);
			Toggle::LastClick = (clock() * 0.00001f) + 0.002f;
			Toggle::ValueChangeEvent();
		}
	}

	if (*Data)
	{
		Toggle::ClickedAnimationProgress += (1.0f / Toggle::ClickedAnimationDuration) * deltaseconds;
		Toggle::ClickedAnimationProgress = std::min(Toggle::ClickedAnimationProgress, 1.0f);
		Toggle::FadeAnimationProgress += (1.0f / Toggle::FadeAnimationDuration) * deltaseconds;
		Toggle::FadeAnimationProgress = std::min(Toggle::FadeAnimationProgress, 1.0f);
	}
	else {
		Toggle::ClickedAnimationProgress -= (1.0f / Toggle::ClickedAnimationDuration) * deltaseconds;
		Toggle::ClickedAnimationProgress = std::max(Toggle::ClickedAnimationProgress, 0.0f);
		Toggle::FadeAnimationProgress -= (1.0f / Toggle::FadeAnimationDuration) * deltaseconds;
		Toggle::FadeAnimationProgress = std::max(Toggle::FadeAnimationProgress, 0.0f);
	}
	MenuColours[LIT("ToggleInside")].a = static_cast<int>(1 + (255 - 1) * Toggle::FadeAnimationProgress);
	int size = static_cast<int>(1 + (Size.x - 2 - 1) * Toggle::ClickedAnimationProgress);
	Toggle::RectangleColour = MenuColours[LIT("ToggleInside")];
	Toggle::ClickedSize = size;
}

void Toggle::Draw()
{
	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;

	MyColour rectColour = MenuColours[LIT("Toggle")];
	MyColour rectOutlineColour = MenuColours[LIT("ToggleOutline")];
	MyColour activeColour = MenuColours[LIT("ToggleInside")];
	MyColour textColour = MenuColours[LIT("Text")];

	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, 1, rectOutlineColour);
	FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x, Size.y, rectColour);
	if (*Toggle::Data == true)
		FilledRectangle(ParentPos.x + Pos.x + 1, ParentPos.y + Pos.y + 1, Toggle::ClickedSize, Toggle::ClickedSize, RectangleColour);

	DrawText(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x) + 3, Toggle::ParentPos.y + Toggle::Pos.y - (Toggle::Size.y / 8), Toggle::Name, LIT("Verdana"), 12, textColour, None);
}
