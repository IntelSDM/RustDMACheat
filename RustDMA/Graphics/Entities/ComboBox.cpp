#include "pch.h"
#include "Input.h"
#include "Drawing.h"
#include "ComboBox.h"
#include "Font.h"

ComboBox::ComboBox(float x, float y, std::wstring text, std::list<bool*> items, std::list<std::wstring> names)
{
	ComboBox::Pos = {x, y};
	ComboBox::Name = text;
	ComboBox::Items = items;
	ComboBox::Names = names;
	ComboBox::Size = {120, 20};
	ComboBox::CalculateBuffer();
	ComboBox::ConvertSelectedName();

	if (ComboBox::MaxVisibleItems > items.size())
	{
		ComboBox::MaxVisibleItems = items.size();
	}
	ComboBox::PointerStart = 0;
	ComboBox::PointerEnd = ComboBox::MaxVisibleItems;
	SetVisible(true);
}

void ComboBox::CalculateBuffer()
{
	if (!ComboBox::Active)
		ComboBox::CutOffBuffer = 15;
	else
		ComboBox::CutOffBuffer = 0;
}

void ComboBox::SetComboBoxWidth()
{
	float width = 0;
	for (std::wstring str : ComboBox::Names)
	{
		float wdth = GetTextSize(str, LIT("Verdana"), 11).x;
		if (wdth > width)
			width = wdth;
	}
	ComboBox::DropWidth = width;
}

void ComboBox::ConvertSelectedName()
{
	std::wstring combinedstr = L"";
	for (int i = 0; i < ComboBox::Names.size(); i++)
	{
		auto it = ComboBox::Names.begin();
		std::advance(it, i);

		auto itbool = ComboBox::Items.begin();
		std::advance(itbool, i);

		if (**itbool == true)
			combinedstr += *it + LIT(L",");
	}
	if (combinedstr.length() == 0)
	{
		combinedstr = LIT(L"Unselected");
		float originalwidth = GetTextSize(combinedstr, LIT("Verdana"), 11).x;
		if (originalwidth < ComboBox::Size.x - ComboBox::CutOffBuffer)
		{
			ComboBox::TextWidth = originalwidth;
			ComboBox::SelectedName = combinedstr;
			return;
		}
		else
		{
			for (int i = combinedstr.length(); i > 0; i--)
			{
				combinedstr.erase(std::prev((combinedstr).end()));
				float width = GetTextSize(combinedstr + LIT(L".."), LIT("Verdana"), 11).x;
				if (width < ComboBox::Size.x - ComboBox::CutOffBuffer)
				{
					ComboBox::SelectedName = combinedstr + LIT(L"..");
					ComboBox::TextWidth = width;
					return;
				}
			}
		}
	}

	combinedstr.erase(std::prev((combinedstr).end())); // last character will be "," and we dont need that
	float originalwidth = GetTextSize(combinedstr, LIT("Verdana"), 11).x;

	if (originalwidth < ComboBox::Size.x - ComboBox::CutOffBuffer)
	{
		ComboBox::SelectedName = combinedstr;
		ComboBox::TextWidth = originalwidth;
		return;
	}
	else
	{
		std::wstring str = combinedstr;
		for (int i = str.length(); i > 0; i--)
		{
			str.erase(std::prev((str).end()));
			float width = GetTextSize(str + LIT(L".."), LIT("Verdana"), 11).x;
			if (width < ComboBox::Size.x - ComboBox::CutOffBuffer)
			{
				ComboBox::SelectedName = str + LIT(L"..");
				ComboBox::TextWidth = width;
				return;
			}
		}
		ComboBox::SelectedName = str + LIT(L"..");
		ComboBox::TextWidth = GetTextSize(str + LIT(L".."), LIT("Verdana"), 11).x;
	}
}

void ComboBox::ArrowNavigation()
{
	if (!ComboBox::Active)
		return;
	if (IsKeyClicked(VK_DOWN) && ComboBox::LastClick < (clock() * 0.00001f))
	{
		if (ComboBox::Names.size() - 1 > ComboBox::PointerEnd)
		{
			ComboBox::PointerEnd++;
			ComboBox::PointerStart++;
			ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}

	if (IsKeyClicked(VK_UP) && ComboBox::LastClick < (clock() * 0.00001f))
	{
		if (ComboBox::PointerStart > 0)
		{
			ComboBox::PointerEnd--;
			ComboBox::PointerStart--;
			ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
}

void ComboBox::Update()
{
	if (!ComboBox::Parent)
		ComboBox::SetVisible(false);
	if (!ComboBox::IsVisible())
		return;

	ComboBox::ArrowNavigation();
	ComboBox::ParentPos = ComboBox::Parent->GetParentPos();
	ComboBox::CalculateBuffer();
	ComboBox::UpdateScrollBar();
	if (!ComboBox::Blocked)
	{
		if (IsMouseInRectangle(ComboBox::Pos + ParentPos, ComboBox::Size) && IsKeyClicked(VK_LBUTTON) && ComboBox::LastClick < (clock() * 0.00001f))
		{
			if (!ComboBox::Active)
			{
				ComboBox::Active = true;
				ComboBox::SetBlockedSiblings(true);
				ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
				ComboBox::CalculateBuffer();
				ComboBox::ConvertSelectedName();
			}
			else
			{
				ComboBox::Active = false;
				ComboBox::SetBlockedSiblings(false);
				ComboBox::CalculateBuffer();
				ComboBox::ConvertSelectedName();
				ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
			}
		}
	}

	if (IsKeyClicked(VK_LBUTTON) && ComboBox::Active && !(IsMouseInRectangle(ComboBox::Pos + ParentPos, ComboBox::Size) || IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x - (ComboBox::SizeDifference / 2), ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::DropWidth, ComboBox::Names.size() * ComboBox::Size.y) ||
		IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x + (ComboBox::SizeDifference / 2), ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 4, 6, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y)))
	{
		ComboBox::Active = false;
		ComboBox::SetBlockedSiblings(false);
		ComboBox::CalculateBuffer();
		ComboBox::ConvertSelectedName();
	}

	if ((IsKeyClicked(VK_RETURN) || IsKeyClicked(VK_ESCAPE)) && ComboBox::Active)
	{
		ComboBox::Active = false;
		ComboBox::SetBlockedSiblings(false);
		ComboBox::CalculateBuffer();
		ComboBox::ConvertSelectedName();
	}

	if (ComboBox::Active)
	{
		ComboBox::SizeDifference = ComboBox::DropWidth - ComboBox::TextWidth;
		ComboBox::SetComboBoxWidth();
		int i = 0;
		for (const std::wstring& name : ComboBox::Names)
		{
			if (i < ComboBox::PointerStart)
			{
				i++;
				continue;
			}
			if (i > ComboBox::PointerEnd - 1)
			{
				i++;
				continue;
			}
			float itemposy = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + ((i - ComboBox::PointerStart) * ComboBox::Size.y);

			if (IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, itemposy, ComboBox::DropWidth + (ComboBox::SizeDifference / 2), ComboBox::Size.y) && IsKeyClicked(VK_LBUTTON) && ComboBox::LastClick < (clock() * 0.00001f))
			{
				auto it = ComboBox::Items.begin();
				std::advance(it, i);
				ComboBox::ValueChangeEvent();
				**it = !**it;
				ComboBox::LastClick = (clock() * 0.00001f) + 0.002f;
				ComboBox::ConvertSelectedName();
				Sleep(50); // bandage click through fix
			}
			i++;
		}
	}
}

void ComboBox::UpdateScrollBar()
{
	if (!IsKeyDown(VK_LBUTTON))
		ComboBox::ScrollBarHeld = false;
	if (IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x + (ComboBox::SizeDifference / 2), ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 4, 6, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y) && IsKeyClicked(VK_LBUTTON))
		ComboBox::ScrollBarHeld = true;

	if (ComboBox::ScrollBarHeld)
	{
		float ratio = (MousePos.y - (float)(ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 4)) / (float)((ComboBox::MaxVisibleItems - 1) * ComboBox::Size.y);
		ratio = std::clamp(ratio, 0.0f, 1.0f);
		ComboBox::PointerEnd = (int)(ComboBox::MaxVisibleItems + (ComboBox::Names.size() - ComboBox::MaxVisibleItems) * ratio);
	}
	ComboBox::PointerStart = ComboBox::PointerEnd - ComboBox::MaxVisibleItems;
}

void ComboBox::Draw()
{
	if (!ComboBox::Parent)
		ComboBox::SetVisible(false);
	if (!ComboBox::IsVisible())
		return;

	MyColour rectColour = MenuColours[LIT("ComboBox")];
	MyColour rectHoverColour = MenuColours[LIT("ComboBoxHover")];
	MyColour rectOutlineColour = MenuColours[LIT("ComboBoxOutline")];
	MyColour textColour = MenuColours[LIT("Text")];
	MyColour selectedTextColour = MenuColours[LIT("ComboBoxSelectedText")];
	MyColour dropdownColour = MenuColours[LIT("ComboBoxDropDown")];
	MyColour sliderColour = MenuColours[LIT("ComboBoxSlider")];

	OutlineRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, ComboBox::ParentPos.y + ComboBox::Pos.y, ComboBox::Size.x + 1, ComboBox::Size.y + 1, 1, rectOutlineColour);
	FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x, ComboBox::ParentPos.y + ComboBox::Pos.y, ComboBox::Size.x, ComboBox::Size.y, rectColour);
	float trianglex1 = ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x - 12;
	float triangley1 = ComboBox::ParentPos.y + ComboBox::Pos.y + 3;
	float trianglex2 = ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x - 3;
	float triangley2 = ComboBox::Pos.y + ComboBox::ParentPos.y + 3;
	float trianglex3 = ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x - 7;
	float triangley3 = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y - 3;

	if (!ComboBox::Active)
		FilledTriangle(trianglex1, triangley1, trianglex2, triangley2, trianglex3, triangley3, dropdownColour);

	DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5, ComboBox::ParentPos.y + ComboBox::Pos.y + (ComboBox::Size.y / 8), ComboBox::SelectedName, LIT("Verdana"), 11, textColour, None);

	if (ComboBox::DropWidth < ComboBox::Size.x)
	{
		ComboBox::DropWidth = ComboBox::Size.x;
		ComboBox::SizeDifference = 0;
	}

	if (ComboBox::Active)
	{
		OutlineRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x - (ComboBox::SizeDifference / 2), ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::DropWidth + 1, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y + 1, 1, rectOutlineColour);
		FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x - (ComboBox::SizeDifference / 2), ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::DropWidth, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y, rectColour);

		int i = 0;
		for (const std::wstring& name : ComboBox::Names)
		{
			if (i < ComboBox::PointerStart)
			{
				i++;
				continue;
			}
			if (i > ComboBox::PointerEnd - 1)
			{
				i++;
				continue;
			}
			float itemposy = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + ((i - ComboBox::PointerStart) * ComboBox::Size.y);
			if (IsMouseInRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x - (ComboBox::SizeDifference / 2), itemposy, ComboBox::DropWidth, ComboBox::Size.y))
			{
				FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x - (ComboBox::SizeDifference / 2), itemposy, ComboBox::DropWidth, ComboBox::Size.y, rectHoverColour);
			}

			auto it = ComboBox::Items.begin();
			std::advance(it, i);

			if (**it == true)
				DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5 - (ComboBox::SizeDifference / 2), itemposy + (ComboBox::Size.y / 8), name, LIT("Verdana"), 11, selectedTextColour, None);
			else
				DrawText(ComboBox::ParentPos.x + ComboBox::Pos.x + 5 - (ComboBox::SizeDifference / 2), itemposy + (ComboBox::Size.y / 8), name, LIT("Verdana"), 11, textColour, None);
			i++;
		}
		OutlineRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x + (ComboBox::SizeDifference / 2), ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, 6, (ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y + 1, 1, rectOutlineColour);
		int unselectedelements = Names.size() - MaxVisibleItems;
		float unselectedclamp = std::clamp(unselectedelements, 1, (int)Names.size());
		float scrollheight = ((ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y) / (unselectedclamp);
		float scrolly = ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + (((PointerEnd - MaxVisibleItems) * Size.y));
		float scrollyclamp = std::clamp(scrolly, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5, ComboBox::ParentPos.y + ComboBox::Pos.y + ComboBox::Size.y + 5 + ((ComboBox::PointerEnd - ComboBox::PointerStart) * ComboBox::Size.y) - scrollheight);
		//Slider (:
		FilledRectangle(ComboBox::ParentPos.x + ComboBox::Pos.x + ComboBox::Size.x + (ComboBox::SizeDifference / 2), scrollyclamp, 5, scrollheight, sliderColour);
	}
}
