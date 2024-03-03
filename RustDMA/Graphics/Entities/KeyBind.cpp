#include "pch.h"
#include "KeyBind.h"
#include "Drawing.h"
#include "Input.h"
#include "GUI.h"

KeyBind::KeyBind(float x, float y, std::wstring text, int* key)
{
	KeyBind::Pos = {x, y};
	KeyBind::Name = text;
	KeyBind::Key = key;
	KeyBind::Size = {80, 20};
	KeyBind::ConvertKeyToName();
	KeyBind::ContextSize = {80.0f, 20.0f * (int)KeyBind::ContextNames.size()};
	SetVisible(true);
}

void KeyBind::ConvertKeyToName()
{
	if (KeyBind::Active)
	{
		KeyBind::KeyName = LIT(L"[Click A Key]");
		return;
	}

	if (*KeyBind::Key == NULL && !KeyBind::Active)
	{
		KeyBind::KeyName = LIT(L"[Unbound]");
		return;
	}

	int keycode = *KeyBind::Key;
	int scancode = MapVirtualKey(keycode, MAPVK_VK_TO_VSC);
	WCHAR keyname[256] = {0};
	if (SymbolKeys[keycode] != L"")
	{
		KeyBind::KeyName = SymbolKeys[keycode];
		return;
	}

	if (GetKeyNameTextW((scancode << 16), keyname, sizeof(keyname) / sizeof(keyname[0])) == 0 && KeyBind::Key != NULL)
	{
		KeyBind::KeyName = LIT(L"[Unknown Key]");
	}
	else
	{
		KeyBind::KeyName = keyname;
	}
}

void KeyBind::Update()
{
	if (!KeyBind::Parent)
		KeyBind::SetVisible(false);
	if (!KeyBind::IsVisible())
		return;

	KeyBind::ContextMenu();
	KeyBind::ParentPos = KeyBind::Parent->GetParentPos();
	if (!KeyBind::Blocked)
	{
		if (IsMouseInRectangle(KeyBind::Pos + ParentPos, KeyBind::Size) && IsKeyClicked(VK_LBUTTON) && KeyBind::LastClick < (clock() * 0.00001f) && !KeyBind::Blocked && !KeyBind::Active)
		{
			//	KeyBind::SetBlockedSiblings(true);
			KeyBind::Blocked = true;
			KeyBind::LastClick = (clock() * 0.00001f) + 0.002f;
			KeyBind::Active = true;
			KeyBind::Blocked = false;
		}
	}
	if (Active && !ContextActive)
	{
		*KeyBind::Key = 0;
		KeyBind::KeyName = LIT(L"[Click A Key]");
		KeyBind::CollectInput();
		KeyBind::ConvertKeyToName();
	}
}

void KeyBind::ContextMenu()
{
	if (IsMouseInRectangle(KeyBind::Pos + KeyBind::ParentPos, KeyBind::Size) && IsKeyClicked(VK_RBUTTON) && !KeyBind::Blocked && !KeyBind::Active && KeyBind::LastClick < (clock() * 0.00001f))
	{
		KeyBind::ContextActive = true;
		KeyBind::Active = false;
		KeyBind::ContextPos = MousePos;
		SetBlockedSiblings(true);
	}

	if (!IsMouseInRectangle(KeyBind::ContextPos, KeyBind::ContextSize) && IsKeyClicked(VK_LBUTTON))
	{
		if (KeyBind::ContextActive)
			SetBlockedSiblings(false);
		KeyBind::ContextActive = false;
	}

	if (!KeyBind::ContextActive)
		return;

	int i = 0;
	for (auto& pair : KeyBind::ContextNames)
	{
		if (IsMouseInRectangle(KeyBind::ContextPos.x, KeyBind::ContextPos.y + (i * 20), KeyBind::ContextSize.x, 20) && IsKeyClicked(VK_LBUTTON) && KeyBind::LastClick < (clock() * 0.00001f))
		{
			pair.second();
			KeyBind::LastClick = (clock() * 0.00001f) + 0.002f;
		}
		i++;
	}
}

void KeyBind::Copy()
{
	KeyBindClipBoard = *KeyBind::Key;
}

void KeyBind::Paste()
{
	*KeyBind::Key = KeyBindClipBoard;
	KeyBind::ConvertKeyToName();
	KeyBind::ValueChangeEvent();
}

void KeyBind::CollectInput()
{
	for (int i = 0; i <= 255; i++)
	{
		if (IsKeyClicked(VK_RETURN) || IsKeyClicked(VK_ESCAPE))
		{
			KeyBind::Active = false;
			return;
		}
		if (IsKeyClicked(i) && KeyBind::LastClick < (clock() * 0.00001f))
		{
			*KeyBind::Key = i;
			KeyBind::Active = false;
			KeyBind::LastClick = (clock() * 0.00001f) + 0.002f;
			KeyBind::ValueChangeEvent();
			return;
		}
		
	}
}

void KeyBind::Draw()
{
	if (!KeyBind::Parent)
		KeyBind::SetVisible(false);
	if (!KeyBind::IsVisible())
		return;

	MyColour rectColour = MenuColours[LIT("Keybind")];
	MyColour rectHoverColour = MenuColours[LIT("KeybindHover")];
	MyColour rectOutlineColour = MenuColours[LIT("KeybindOutline")];
	MyColour textColour = MenuColours[LIT("Text")];
	MyColour textselected = MenuColours[LIT("TextSelected")];

	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, 1, rectOutlineColour);
	FilledRectangle(ParentPos.x + KeyBind::Pos.x, ParentPos.y + KeyBind::Pos.y, KeyBind::Size.x, KeyBind::Size.y, rectColour);
	DrawText(ParentPos.x + KeyBind::Pos.x, ParentPos.y + KeyBind::Pos.y - KeyBind::Size.y, KeyBind::Name, LIT("Verdana"), 12, textColour, None);
	DrawText(ParentPos.x + KeyBind::Pos.x + (KeyBind::Size.x / 2), ParentPos.y + KeyBind::Pos.y + (KeyBind::Size.y / 2), KeyName, LIT("Verdana"), 10, Active ? textselected : textColour, CentreCentre);
	if (KeyBind::ContextActive)
	{
		OutlineRectangle(KeyBind::ContextPos.x, KeyBind::ContextPos.y, KeyBind::ContextSize.x, KeyBind::ContextSize.y, 1, textColour);
		FilledRectangle(KeyBind::ContextPos.x, KeyBind::ContextPos.y, KeyBind::ContextSize.x, KeyBind::ContextSize.y, rectColour);
		int i = 0;
		for (auto pair : KeyBind::ContextNames)
		{
			if (i != 0)
				FilledLine(KeyBind::ContextPos.x, KeyBind::ContextPos.y + i * 20, KeyBind::ContextPos.x + KeyBind::ContextSize.x, KeyBind::ContextPos.y + i * 20, 1.0f, textColour);

			if (IsMouseInRectangle(KeyBind::ContextPos.x, KeyBind::ContextPos.y + (i * 20), KeyBind::ContextSize.x, 20))
				FilledRectangle(KeyBind::ContextPos.x, KeyBind::ContextPos.y + (i * 20), KeyBind::ContextSize.x, 20, rectHoverColour);
			DrawText(KeyBind::ContextPos.x + (KeyBind::ContextSize.x / 2), KeyBind::ContextPos.y + (i * 20) + 10, pair.first, LIT("Verdana"), 11, textColour, CentreCentre);

			i++;
		}
	}
}
