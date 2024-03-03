#include "Pch.h"
#include "TabListBoxController.h"
#include "GUI.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"
#include "Graphics.h"
#include "TabListBox.h"

TabListBoxController::TabListBoxController(float x, float y, float width, float height)
{
	TabListBoxController::Pos = {x, y};
	TabListBoxController::Size = {width, height};
	TabListBoxController::Selected = 0;
	TabListBoxController::PointerEnd = TabListBoxController::Size.y / 20;
	TabListBoxController::PointerStart = 0;
	TabListBoxController::SetActiveIndex();
	TabListBoxController::CachedHeight = height;
	SetVisible(true);
}

void TabListBoxController::SetActiveIndex()
{
	int i = 0;
	for (auto tab : TabListBoxController::Tabs)
	{
		if (tab->Index == TabListBoxController::Selected)
		{
			TabListBoxController::ActiveIndex = i;
			return;
		}
		i++;
	}
}

void TabListBoxController::UpdateCulledNames()
{
	CulledNames.clear();
	for (std::wstring str : TabListBoxController::Names)
	{
		std::wstring culledname = LIT(L"");
		float width = GetTextSize(str, LIT("Verdana"), 11).x;
		if (width < TabListBoxController::Size.x - TabListBoxController::ScrollWidth + 2)
		{
			CulledNames.push_back(str);
		}
		else
		{
			culledname = str;
			for (int i = culledname.length(); i > 0; i--)
			{
				culledname.erase(std::prev((culledname).end()));
				float width = GetTextSize(culledname +LIT( L".."), LIT("Verdana"), 11).x;
				if (width < TabListBoxController::Size.x - TabListBoxController::ScrollWidth + 2)
				{
					CulledNames.push_back(culledname + LIT(L".."));
					break;
				}
			}
		}
	}
}

void TabListBoxController::ArrowKeyNavigation()
{
	if (TabListBoxController::Tabs.size() < TabListBoxController::Size.y / 20)
		return;
	if (!Active)
		return;
	if (!((TabListBoxController::Tabs.size() + 1) * 20 > TabListBoxController::Size.y / 20 && (TabListBoxController::Tabs.size() + 1) * 20 > TabListBoxController::CachedHeight))
		return;
	if (IsKeyClicked(VK_DOWN) && TabListBoxController::LastClick < (clock() * 0.00001f))
	{
		if (TabListBoxController::Tabs.size() > TabListBoxController::PointerEnd)
		{
			TabListBoxController::PointerEnd++;
			TabListBoxController::PointerStart++;
			TabListBoxController::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
	if (IsKeyClicked(VK_UP) && TabListBoxController::LastClick < (clock() * 0.00001f))
	{
		if (TabListBoxController::PointerStart > 0)
		{
			TabListBoxController::PointerEnd--;
			TabListBoxController::PointerStart--;
			TabListBoxController::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
}

void TabListBoxController::SetActive()
{
	if (IsMouseInRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, TabListBoxController::ParentPos.y + TabListBoxController::Pos.y, TabListBoxController::Size.x, TabListBoxController::Size.y))
		Active = true;
	else
		Active = false;
}

void TabListBoxController::Update()
{
	if (!TabListBoxController::Parent)
		TabListBoxController::SetVisible(false);
	if (!TabListBoxController::IsVisible())
		return;

	TabListBoxController::ParentPos = TabListBoxController::Parent->GetParentPos();
	TabListBoxController::ArrowKeyNavigation();
	TabListBoxController::ScrollBarAction();
	TabListBoxController::SetActive();
	int i = 0;
	for (auto tab : TabListBoxController::Tabs)
	{
		if (i < TabListBoxController::PointerStart)
		{
			i++;
			continue;
		}
		if (i > TabListBoxController::PointerEnd)
		{
			i++;
			continue;
		}
		float itemposy = TabListBoxController::ParentPos.y + TabListBoxController::Pos.y + ((i - TabListBoxController::PointerStart) * 20);
		if (IsMouseInRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2, (itemposy), TabListBoxController::Size.x - (TabListBoxController::ScrollWidth + 2), 20) && TabListBoxController::LastClick < (clock() * 0.00001f) && IsKeyClicked(VK_LBUTTON))
		{
			TabListBoxController::ValueChangeEvent();
			Selected = tab->Index;
			TabListBoxController::SetActiveIndex();
			TabListBoxController::LastClick = (clock() * 0.00001f) + 0.002f;
		}
		i++;
	}
}

void TabListBoxController::ScrollBarAction()
{
	if (TabListBoxController::Tabs.size() < TabListBoxController::Size.y / 20)
		return;
	if (!((TabListBoxController::Tabs.size() + 1) * 20 > TabListBoxController::Size.y / 20 && (TabListBoxController::Tabs.size() + 1) * 20 > TabListBoxController::CachedHeight))
		return;
	if (!IsKeyDown(VK_LBUTTON))
		TabListBoxController::ScrollBarHeld = false;
	if (IsMouseInRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, TabListBoxController::ParentPos.y + TabListBoxController::Pos.y, 5, TabListBoxController::Size.y) && IsKeyClicked(VK_LBUTTON))
		TabListBoxController::ScrollBarHeld = true;
	if (TabListBoxController::ScrollBarHeld)
	{
		float ratio = (MousePos.y - (float)(TabListBoxController::ParentPos.y + TabListBoxController::Pos.y)) / (float)((TabListBoxController::MaxVisibleItems - 1) * 20);
		ratio = std::clamp(ratio, 0.0f, 1.0f);
		TabListBoxController::PointerEnd = (int)(TabListBoxController::MaxVisibleItems + (TabListBoxController::Names.size() - TabListBoxController::MaxVisibleItems) * ratio);
	}
	TabListBoxController::PointerStart = TabListBoxController::PointerEnd - TabListBoxController::MaxVisibleItems;
}

int TabListBoxController::GetActiveIndex()
{
	return TabListBoxController::ActiveIndex;
}

void TabListBoxController::Draw()
{
	if (!TabListBoxController::IsVisible())
		return;
	for (std::shared_ptr<TabListBox> tab : TabListBoxController::Tabs)
	{
		if (tab->Index == Selected)
		{
			tab->Draw();
			tab->Update();
		}
	}
	FilledRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, TabListBoxController::ParentPos.y + TabListBoxController::Pos.y, TabListBoxController::Size.x, TabListBoxController::Size.y, Colour(80, 80, 80, 255));
	OutlineRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, TabListBoxController::ParentPos.y + TabListBoxController::Pos.y, TabListBoxController::Size.x + 1, TabListBoxController::Size.y + 1, 1, Colour(150, 150, 150, 255));

	int i = 0;
	for (std::wstring culledname : TabListBoxController::CulledNames)
	{
		if (i < TabListBoxController::PointerStart)
		{
			i++;
			continue;
		}
		if (i > TabListBoxController::PointerEnd)
		{
			i++;
			continue;
		}
		float itemposy = TabListBoxController::ParentPos.y + TabListBoxController::Pos.y + ((i - TabListBoxController::PointerStart) * 20);
		if (i % 2 == 0)
			FilledRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, (itemposy), TabListBoxController::Size.x, 20, Colour(30, 30, 30, 255));
		else
			FilledRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, (itemposy), TabListBoxController::Size.x, 20, Colour(50, 50, 50, 255));
		if (!IsMouseInRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2, (itemposy), TabListBoxController::Size.x - (TabListBoxController::ScrollWidth + 2), 20))
		{
			if (i == TabListBoxController::ActiveIndex)
			{
				DrawText(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2, (itemposy), culledname, LIT("Verdana"), 11, Colour(0, 255, 150, 255), None);
			}
			else
				DrawText(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2, (itemposy), culledname, LIT("Verdana"), 11, Colour(255, 255, 255, 255), None);
		}
		i++;
	}
	// do this in a seperate loop so we can draw over all culled names
	i = 0;
	for (std::wstring name : TabListBoxController::Names)
	{
		if (i < TabListBoxController::PointerStart)
		{
			i++;
			continue;
		}
		if (i > TabListBoxController::PointerEnd)
		{
			i++;
			continue;
		}
		float itemposy = TabListBoxController::ParentPos.y + TabListBoxController::Pos.y + ((i - TabListBoxController::PointerStart) * 20);
		if (IsMouseInRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2, (itemposy), TabListBoxController::Size.x - (TabListBoxController::ScrollWidth + 2), 20))
		{
			int width = GetTextSize(name, LIT("Verdana"), 11).x;
			if (width + TabListBoxController::ScrollWidth + 2 + 5 < TabListBoxController::Size.x)
			{
				FilledRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, (itemposy), TabListBoxController::Size.x, 20, Colour(120, 120, 120, 255));
				if (i == TabListBoxController::ActiveIndex)
					DrawText(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2 + 5, (itemposy) + 5, name, LIT("Verdana"), 11, Colour(0, 255, 150, 255), None);
				else
					DrawText(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2 + 5, (itemposy), name, LIT("Verdana"), 11, Colour(255, 255, 255, 255), None);
			}
			else
			{
				FilledRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2 + 5, (itemposy), width, 20, Colour(120, 120, 120, 255));
				if (i == TabListBoxController::ActiveIndex)
					DrawText(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2 + 5, (itemposy) + 5, name, LIT("Verdana"), 11, Colour(0, 255, 150, 255), None);
				else
					DrawText(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x + TabListBoxController::ScrollWidth + 2 + 5, (itemposy) + 5, name, LIT("Verdana"), 11, Colour(255, 255, 255, 255), None);
			}
		}

		i++;
	}
	if ((TabListBoxController::Tabs.size() + 1) * 20 > TabListBoxController::Size.y / 20 && (TabListBoxController::Tabs.size() + 1) * 20 > TabListBoxController::CachedHeight)
	{
		int unselectedelements = Tabs.size() - MaxVisibleItems;
		float unselectedclamp = std::clamp(unselectedelements, 1, (int)Names.size());
		float scrollheight = (TabListBoxController::Size.y / unselectedclamp) * 1.2;
		float scrolly = TabListBoxController::ParentPos.y + TabListBoxController::Pos.y + (((PointerEnd - MaxVisibleItems) * 20));
		float scrollyclamp = std::clamp(scrolly, TabListBoxController::ParentPos.y + TabListBoxController::Pos.y, TabListBoxController::ParentPos.y + TabListBoxController::Pos.y + 5 + ((TabListBoxController::PointerEnd - TabListBoxController::PointerStart) * 20) - scrollheight);

		FilledRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, TabListBoxController::ParentPos.y + TabListBoxController::Pos.y, TabListBoxController::ScrollWidth, TabListBoxController::Size.y, Colour(130, 130, 130, 255));
		FilledRectangle(TabListBoxController::ParentPos.x + TabListBoxController::Pos.x, scrollyclamp, 5, scrollheight, Colour(0, 255, 150, 255));
	}
	else
		TabListBoxController::ScrollWidth = 5; // no scroll bar, no scrollwidth
}

void TabListBoxController::PushBack(std::shared_ptr<TabListBox> tab)
{
	TabListBoxController::Tabs.push_back(tab);
	TabListBoxController::Names.push_back(tab->GetName());
	if (!FirstItem)
	{
		TabListBoxController::Selected = tab->Index;
		FirstItem = true;
	}
	TabListBoxController::Push(tab);
	TabListBoxController::UpdateCulledNames();
	TabListBoxController::PointerEnd = TabListBoxController::Tabs.size();
	TabListBoxController::MaxVisibleItems = TabListBoxController::Tabs.size();
	if (TabListBoxController::Tabs.size() > TabListBoxController::Size.y / 20)
		TabListBoxController::MaxVisibleItems = TabListBoxController::Size.y / 20;
	if (TabListBoxController::PointerEnd > TabListBoxController::Size.y / 20)
		TabListBoxController::PointerEnd = TabListBoxController::Size.y / 20;

	if (((TabListBoxController::Names.size()) * 20) < TabListBoxController::CachedHeight)
		TabListBoxController::Size.y = ((TabListBoxController::Names.size()) * 20);
}
