#include "pch.h"
#include "TextBox.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"
#include "Graphics.h"
#include "Animation.h"

TextBox::TextBox(float x, float y, std::wstring text, std::wstring* data = nullptr, bool hide)
{
	TextBox::Pos = { x, y };
	TextBox::Size = { 160, 20 };
	TextBox::Name = text;
	//TextBox::Blocked = false;
	TextBox::MainString = data;
	TextBox::VisiblePointerEnd = MainString->length();
	TextBox::SetStartIndex(); // this sets start value
	TextBox::VisibleString = MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd);
	TextBox::SelectedPoint = VisiblePointerEnd - TextBox::VisiblePointerStart;
	TextBox::SelectedPosition = GetTextSize(TextBox::MainString->substr(TextBox::VisiblePointerStart, TextBox::SelectedPoint), "Verdana").x;
	TextBox::ContextSize = { 80.0f, 20.0f * (int)TextBox::ContextNames.size() };
	TextBox::hide_text = hide;
	TextBox::SetVisible(true);
}

void TextBox::SetStartIndex()
{
	// Sets the value to be the right most character at the end.
	TextBox::VisiblePointerStart = 0;
	TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana").x;
	while (TextBox::TextWidth > TextBox::Size.x - 6)
	{
		TextBox::VisiblePointerStart++; // update position
		TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana").x; // update width so we can exit
	}
}

void TextBox::SetState()
{
	WPARAM character = Char;
	if (IsMouseInRectangle(TextBox::Pos + TextBox::ParentPos, TextBox::Size) && IsKeyClicked(VK_LBUTTON) && !TextBox::Blocked && ((TextBox::ContextActive && !IsMouseInRectangle(TextBox::ContextPos, TextBox::ContextSize)) || !TextBox::ContextActive))
	{
		TextBox::Active = true;
		TextBox::ContextActive = false;
		Char = NULL;
	}
	else if (IsKeyClicked(VK_LBUTTON) && !IsMouseInRectangle(TextBox::Pos + TextBox::ParentPos, TextBox::Size) && TextBox::Active)
	{
		TextBox::Selecting = false;
		TextBox::Held = false;
		TextBox::Active = false; // prevent 2 being active at the same time unless they are somehow fucking merged
		TextBox::ValueChangeEvent();
	}
	else if ((character == VK_RETURN || character == VK_ESCAPE) && TextBox::Active)
	{
		TextBox::Active = false;
		TextBox::ValueChangeEvent();
		TextBox::Selecting = false;
		Char = NULL;
	}

	if (!IsKeyDown(VK_LBUTTON))
		TextBox::Held = false;
	if (TextBox::SelectedPoint == TextBox::SelectionStart && TextBox::SelectedPoint == TextBox::SelectionEnd)
		TextBox::Selecting = false;
}

bool TextBox::IsKeyAcceptable()
{
	WPARAM character = Char;
	if (character > 255)
		return false;
	if (character == NULL)
		return false;
	if (character == VK_BACK)
	{
		DeleteText();
		return false;
	}
	if (character == VK_RETURN)
		return false;
	if (IsKeyDown(VK_CONTROL))
		return false;
	if (character == VK_TAB)
		return false;
	return true;
}

void TextBox::ArrowKeyNavition()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	if (IsKeyClicked(VK_LEFT) && TextBox::LastClick < (clock() * 0.00001f))
	{
		TextBox::Selecting = false;
		TextBox::Held = false;
		// pointer isn't behind visible text
		if (SelectedPoint > TextBox::VisiblePointerStart)
		{
			TextBox::SelectedPoint--;
		}
		// pointer is going behind currently visible text
		else if (TextBox::VisiblePointerStart != 0 && SelectedPoint == TextBox::VisiblePointerStart)
		{
			TextBox::SelectedPoint--;
			TextBox::VisiblePointerStart--;
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			// if the value exceeds the textbox bounds decrement the ending
			while (TextBox::TextWidth > TextBox::Size.x - 6 && TextBox::VisiblePointerStart != 0)
			{
				TextBox::VisiblePointerEnd--;
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
			while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::MainString->length() > TextBox::VisiblePointerEnd && TextBox::VisiblePointerStart == 0)
			{
				TextBox::VisiblePointerEnd++; // update position
				TextBox::SelectedPoint++;
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		Char = NULL;
		TextBox::LastClick = (clock() * 0.00001f) + 0.002f;
	}
	if (IsKeyClicked(VK_RIGHT) && TextBox::LastClick < (clock() * 0.00001f))
	{
		TextBox::Selecting = false;
		TextBox::Held = false;
		if (TextBox::SelectedPoint < TextBox::VisiblePointerEnd)
			TextBox::SelectedPoint++;

		else if (TextBox::VisiblePointerEnd != TextBox::MainString->length() && TextBox::SelectedPoint == TextBox::VisiblePointerEnd)
		{
			TextBox::SelectedPoint++;
			TextBox::VisiblePointerEnd++;
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			// decrement start
			while (TextBox::TextWidth > TextBox::Size.x - 6)
			{
				TextBox::VisiblePointerStart++; // update position
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		Char = NULL;
		TextBox::LastClick = (clock() * 0.00001f) + 0.002f;
	}
}

void TextBox::InputText()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	if (TextBox::IsKeyAcceptable() && std::isprint(Char))
	{
		Selecting = false;
		TextBox::VisiblePointerEnd++;
		TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
		MainString->insert(TextBox::SelectedPoint, 1, Char);
		TextBox::SelectedPoint++;
		while (TextBox::TextWidth > TextBox::Size.x - 6)
		{
			TextBox::VisiblePointerStart++; // update position
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
		}

	}
	Char = NULL;
}

void TextBox::DeleteText()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	WPARAM character = Char;
	if (character == VK_BACK && (*TextBox::MainString).length() != 0 && TextBox::VisiblePointerEnd != 0 && SelectedPoint != 0) // backspace
	{
		// no selection
		if (TextBox::SelectionStart == TextBox::SelectedPoint && TextBox::SelectionEnd == TextBox::SelectedPoint && !TextBox::Selecting)
		{
			if (TextBox::SelectedPoint == TextBox::VisiblePointerEnd)
			{
				(*TextBox::MainString).erase(std::prev((*TextBox::MainString).end()));
				TextBox::VisiblePointerEnd--;
				TextBox::SelectedPoint--;
			}
			else
			{
				TextBox::MainString->erase(TextBox::SelectedPoint - 1, 1);
				TextBox::SelectedPoint--;
				TextBox::VisiblePointerEnd--;
			}

			if (TextBox::VisiblePointerStart != 0 && GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x < TextBox::Size.x - 6)
			{
				TextBox::VisiblePointerStart--;
			}
			// detect if there is any other text that we might need to add so our string doesn't randomly get cut off
			while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::MainString->length() > TextBox::VisiblePointerEnd)
			{
				TextBox::VisiblePointerEnd++; // update position
				TextBox::SelectedPoint++;
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart + 1, TextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		else
		{
			//selecting
			// only change selectedpoint if its going to be removed as well
			if (TextBox::SelectedPoint == TextBox::SelectionEnd)
			{
				TextBox::MainString->erase(TextBox::SelectionStart, TextBox::SelectionEnd - TextBox::SelectionStart);
				TextBox::VisiblePointerEnd -= TextBox::SelectionEnd - TextBox::SelectionStart;
				TextBox::SelectedPoint -= TextBox::SelectionEnd - TextBox::SelectionStart;
			}
			else
			{
				TextBox::MainString->erase(TextBox::SelectionStart, TextBox::SelectionEnd - TextBox::SelectionStart);
				TextBox::VisiblePointerEnd -= TextBox::SelectionEnd - TextBox::SelectionStart;
			}
			while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::VisiblePointerStart > 0)
			{
				TextBox::VisiblePointerStart--; // Move the starting point up
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			// If the text still doesn't fill the TextBox, try to extend from the end
			while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::VisiblePointerEnd < TextBox::MainString->length())
			{
				TextBox::VisiblePointerEnd++; // Extend the ending point
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			//reset selected points
			TextBox::SelectionStart = TextBox::SelectedPoint;
			TextBox::SelectionEnd = TextBox::SelectedPoint;
			TextBox::Held = false;
			TextBox::Selecting = false;
		}
		Char = NULL;
	}
}

/*void TextBox::ClearText()
{
	Char = NULL;
}*/

void TextBox::SetSelection()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	if (TextBox::Held)
	{
		TextBox::Selecting = true;
		Vector2 relativemousepos = { MousePos.x - (TextBox::Pos.x + TextBox::ParentPos.x), MousePos.y - (TextBox::Pos.y + TextBox::ParentPos.y) };
		float lastdistance = 99999; // the user shouldn't ever have a resolution/position over this value
		int instance = 0;
		for (int i = TextBox::VisiblePointerStart; i <= TextBox::VisiblePointerEnd; i++)
		{
			float width = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, i - TextBox::VisiblePointerStart), "Verdana", 11).x;
			float distance = std::abs(relativemousepos.x - width);
			if (distance > lastdistance)
			{
				instance = --i;
				break;
			}
			lastdistance = distance;
			instance = i;
		}
		if (instance == -1)
			instance = TextBox::TextBox::VisiblePointerEnd;
		if (TextBox::SelectedPoint > instance)
		{
			TextBox::SelectionEnd = TextBox::SelectedPoint;
			TextBox::SelectionStart = instance;
		}
		else
		{
			TextBox::SelectionEnd = instance;
			TextBox::SelectionStart = TextBox::SelectedPoint;
		}
	}

	if (IsKeyDown(VK_CONTROL) && IsKeyDown(0x41)) //(A)
	{
		TextBox::SelectionStart = 0;
		TextBox::SelectionEnd = MainString->length();
	}
}

void TextBox::ContextSelectAll()
{
	TextBox::SelectionStart = 0;
	TextBox::SelectionEnd = MainString->length();
}

void TextBox::ContextCopyText()
{
	if (!OpenClipboard(nullptr))
		return;
	size_t size = (SelectionEnd - SelectionStart) * sizeof(wchar_t) + sizeof(wchar_t);

	HGLOBAL global = GlobalAlloc(GMEM_MOVEABLE, size);
	if (!global)
	{
		CloseClipboard();
		return;
	}

	wchar_t* text = static_cast<wchar_t*>(GlobalLock(global));
	if (!text)
	{
		CloseClipboard();
		GlobalFree(global);
		return;
	}

	wcsncpy_s(text, size / sizeof(wchar_t), MainString->substr(SelectionStart, SelectionEnd - SelectionStart).c_str(), SelectionEnd - SelectionStart);

	text[SelectionEnd - SelectionStart] = L'\0';
	GlobalUnlock(global);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, global);
	CloseClipboard();
}

void TextBox::ContextPasteText()
{
	if (!OpenClipboard(nullptr))
		return;
	std::wstring clipboard = L"";
	HANDLE data = GetClipboardData(CF_UNICODETEXT);
	if (data != nullptr)
	{
		wchar_t* text = static_cast<wchar_t*>(GlobalLock(data));
		if (text != nullptr)
		{
			clipboard = text;
			GlobalUnlock(data);
		}
	}

	CloseClipboard();

	if (TextBox::SelectedPoint == TextBox::SelectionStart && TextBox::SelectedPoint == TextBox::SelectionEnd)
	{
		TextBox::VisiblePointerEnd += clipboard.length();
		MainString->insert(TextBox::SelectedPoint, clipboard);
		TextBox::SelectedPoint += clipboard.length();
		TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
		while (TextBox::TextWidth > TextBox::Size.x - 6)
		{
			TextBox::VisiblePointerStart++; // update position
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
		}
	}
	else
	{
		if (TextBox::SelectedPoint == TextBox::SelectionEnd)
		{
			TextBox::MainString->erase(TextBox::SelectionStart, TextBox::SelectionEnd - TextBox::SelectionStart);
			TextBox::VisiblePointerEnd -= TextBox::SelectionEnd - TextBox::SelectionStart;
			TextBox::SelectedPoint -= TextBox::SelectionEnd - TextBox::SelectionStart;
		}
		else
		{
			TextBox::MainString->erase(TextBox::SelectionStart, TextBox::SelectionEnd - TextBox::SelectionStart);
			TextBox::VisiblePointerEnd -= TextBox::SelectionEnd - TextBox::SelectionStart;
		}
		TextBox::VisiblePointerEnd += clipboard.length();
		MainString->insert(TextBox::SelectedPoint, clipboard);
		TextBox::SelectedPoint += clipboard.length();
		TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
		while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::VisiblePointerStart > 0)
		{
			TextBox::VisiblePointerStart--; // Move the starting point up
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
		}

		// If the text still doesn't fill the TextBox, try to extend from the end
		while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::VisiblePointerEnd < TextBox::MainString->length())
		{
			TextBox::VisiblePointerEnd++; // Extend the ending point
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
		}

		//reset selected points
		TextBox::SelectionStart = TextBox::SelectedPoint;
		TextBox::SelectionEnd = TextBox::SelectedPoint;
	}
}

void TextBox::SetSelectionPoint()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	if (IsMouseInRectangle(TextBox::Pos + TextBox::ParentPos, TextBox::Size) && IsKeyClicked(VK_LBUTTON))
	{
		TextBox::Held = true;
		Vector2 relativemousepos = { MousePos.x - (TextBox::Pos.x + TextBox::ParentPos.x), MousePos.y - (TextBox::Pos.y + TextBox::ParentPos.y) };
		// get width
		// get last position
		// is last position closer or this one?
		// if last position is closer return last position.
		float lastdistance = 999999;
		int instance = 0;
		for (int i = TextBox::VisiblePointerStart; i <= TextBox::VisiblePointerEnd; i++)
		{
			float width = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, i - TextBox::VisiblePointerStart), "Verdana", 11).x;
			float distance = std::abs(relativemousepos.x - width);
			if (distance > lastdistance)
			{
				instance = --i;
				break;
			}
			lastdistance = distance;
			instance = i;
		}
		if (instance == -1)
			instance = TextBox::VisiblePointerEnd;
		TextBox::SelectedPoint = instance;
	}
}

void TextBox::SelectionDragging()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	if (TextBox::Held)
	{
		const int delay = 100; // Set the desired delay in milliseconds
		auto time = std::chrono::high_resolution_clock::now();
		// push text forwards
		while (TextBox::VisiblePointerEnd != MainString->length() && TextBox::SelectionEnd == TextBox::VisiblePointerEnd)
		{
			auto curtime = std::chrono::high_resolution_clock::now();
			auto elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - time).count();
			if (elapsedtime >= delay)
			{
				TextBox::SelectedPoint++;
				TextBox::VisiblePointerEnd++;
				TextBox::VisiblePointerStart++;
			}
		}

		while (TextBox::VisiblePointerStart != 0 && TextBox::SelectionStart == TextBox::VisiblePointerStart)
		{
			auto curtime = std::chrono::high_resolution_clock::now();
			auto elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - time).count();
			if (elapsedtime >= delay)
			{
				TextBox::SelectedPoint--;
				TextBox::VisiblePointerEnd--;
				TextBox::VisiblePointerStart--;
				time = std::chrono::high_resolution_clock::now();
			}
		}
	}
}

void TextBox::CopyText()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	if (TextBox::SelectedPoint == TextBox::SelectionStart && TextBox::SelectedPoint == TextBox::SelectionEnd)
		return;
	if (!(IsKeyDown(VK_CONTROL) && IsKeyDown(0x43)))
		return;
	if (!OpenClipboard(nullptr))
		return;
	size_t size = (SelectionEnd - SelectionStart) * sizeof(wchar_t) + sizeof(wchar_t);

	HGLOBAL global = GlobalAlloc(GMEM_MOVEABLE, size);
	if (!global)
	{
		CloseClipboard();
		return;
	}

	wchar_t* text = static_cast<wchar_t*>(GlobalLock(global));
	if (!text)
	{
		CloseClipboard();
		GlobalFree(global);
		return;
	}

	wcsncpy_s(text, size / sizeof(wchar_t), MainString->substr(SelectionStart, SelectionEnd - SelectionStart).c_str(), SelectionEnd - SelectionStart);

	text[SelectionEnd - SelectionStart] = L'\0';
	GlobalUnlock(global);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, global);
	CloseClipboard();
}

void TextBox::PasteText()
{
	if (TextBox::Blocked)
		return;
	if (!TextBox::Active)
		return;
	if (!(IsKeyDown(VK_CONTROL) && IsKeyDown(0x56)))
		return;
	if (!OpenClipboard(nullptr))
		return;
	std::wstring clipboard = L"";
	HANDLE data = GetClipboardData(CF_UNICODETEXT);
	if (data != nullptr)
	{
		wchar_t* text = static_cast<wchar_t*>(GlobalLock(data));
		if (text != nullptr)
		{
			clipboard = text;
			GlobalUnlock(data);
		}
	}

	CloseClipboard();
	if (TextBox::LastClick < (clock() * 0.00001f))
	{
		if (TextBox::SelectedPoint == TextBox::SelectionStart && TextBox::SelectedPoint == TextBox::SelectionEnd)
		{
			TextBox::VisiblePointerEnd += clipboard.length();
			MainString->insert(TextBox::SelectedPoint, clipboard);
			TextBox::SelectedPoint += clipboard.length();
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			while (TextBox::TextWidth > TextBox::Size.x - 6)
			{
				TextBox::VisiblePointerStart++; // update position
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		else
		{
			if (TextBox::SelectedPoint == TextBox::SelectionEnd)
			{
				TextBox::MainString->erase(TextBox::SelectionStart, TextBox::SelectionEnd - TextBox::SelectionStart);
				TextBox::VisiblePointerEnd -= TextBox::SelectionEnd - TextBox::SelectionStart;
				TextBox::SelectedPoint -= TextBox::SelectionEnd - TextBox::SelectionStart;
			}
			else
			{
				TextBox::MainString->erase(TextBox::SelectionStart, TextBox::SelectionEnd - TextBox::SelectionStart);
				TextBox::VisiblePointerEnd -= TextBox::SelectionEnd - TextBox::SelectionStart;
			}
			TextBox::VisiblePointerEnd += clipboard.length();
			MainString->insert(TextBox::SelectedPoint, clipboard);
			TextBox::SelectedPoint += clipboard.length();
			TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::VisiblePointerStart > 0)
			{
				TextBox::VisiblePointerStart--; // Move the starting point up
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			// If the text still doesn't fill the TextBox, try to extend from the end
			while (TextBox::TextWidth < TextBox::Size.x - 6 && TextBox::VisiblePointerEnd < TextBox::MainString->length())
			{
				TextBox::VisiblePointerEnd++; // Extend the ending point
				TextBox::TextWidth = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			//reset selected points
			TextBox::SelectionStart = TextBox::SelectedPoint;
			TextBox::SelectionEnd = TextBox::SelectedPoint;
		}
		TextBox::LastClick = (clock() * 0.00001f) + 0.002f;
	}
}

void TextBox::ContextMenu()
{
	if (IsMouseInRectangle(TextBox::Pos + TextBox::ParentPos, TextBox::Size) && IsKeyClicked(VK_RBUTTON) && !TextBox::Blocked)
	{
		TextBox::ContextActive = true;
		TextBox::ContextPos = MousePos;
		TextBox::Selecting = false;
		TextBox::Held = false;
		TextBox::Active = false; // prevent 2 being active at the same time unless they are somehow fucking merged
		TextBox::LastClick = (clock() * 0.00001f) + 0.002f;
		TextBox::ValueChangeEvent();
		TextBox::SetBlockedSiblings(true);
		Sleep(50);
	}
	else if (IsKeyClicked(VK_LBUTTON) && !IsMouseInRectangle(TextBox::Pos + TextBox::ParentPos, TextBox::Size) && TextBox::ContextActive)
	{
		TextBox::ContextActive = false;
		TextBox::SetBlockedSiblings(false);
		Sleep(50);
	}
	if (!(IsMouseInRectangle(TextBox::Pos + TextBox::ParentPos, TextBox::Size) || IsMouseInRectangle(TextBox::ContextPos, TextBox::ContextSize)) && IsKeyClicked(VK_RBUTTON) && !TextBox::Blocked && TextBox::ContextActive)
	{
		TextBox::SetBlockedSiblings(false);
		TextBox::ContextActive = false;
		Sleep(50);
	}
	if (!TextBox::ContextActive)
		return;
	int i = 0;
	for (auto& pair : TextBox::ContextNames)
	{
		if (IsMouseInRectangle(TextBox::ContextPos.x, TextBox::ContextPos.y + (i * 20), TextBox::ContextSize.x, 20) && IsKeyClicked(VK_LBUTTON) && TextBox::LastClick < (clock() * 0.00001f))
		{
			pair.second();
			TextBox::LastClick = (clock() * 0.00001f) + 0.002f;
			TextBox::SetBlockedSiblings(false);
		}
		i++;
	}
}

void TextBox::Update()
{
	if (!TextBox::Parent)
		TextBox::SetVisible(false);
	if (!TextBox::IsVisible())
		return;

	TextBox::TextWidth = GetTextSize(VisibleString, "Verdana", 11).x;
	TextBox::ParentPos = TextBox::Parent->GetParentPos();
	TextBox::VisibleString = MainString->substr(TextBox::VisiblePointerStart, TextBox::VisiblePointerEnd);
	TextBox::SetState();
	TextBox::ArrowKeyNavition();
	TextBox::InputText();
	//	TextBox::DeleteText();
		//TextBox::ClearText();
	TextBox::SetSelectionPoint();
	TextBox::SetSelection();
	TextBox::SelectionDragging();
	TextBox::CopyText();
	TextBox::PasteText();
	TextBox::ContextMenu();

	if (TextBox::Active) // take input
	{
		if (!TextBox::Held && !TextBox::Selecting)
		{
			TextBox::SelectionStart = TextBox::SelectedPoint;
			TextBox::SelectionEnd = TextBox::SelectedPoint;
		}

		// Update the selected point if it is out of bounds
		if (TextBox::SelectedPoint > TextBox::VisiblePointerEnd)
		{
			TextBox::SelectedPoint = TextBox::VisiblePointerEnd;
		}

		if (TextBox::hide_text)
		{
			std::fill(TextBox::VisibleString.begin(), TextBox::VisibleString.end(), L'*');
			TextBox::SelectedPosition = GetTextSize(TextBox::VisibleString.substr(TextBox::VisiblePointerStart, TextBox::SelectedPoint - TextBox::VisiblePointerStart), "Verdana", 11).x;
			TextBox::SelectingStartPosition = GetTextSize(TextBox::VisibleString.substr(TextBox::VisiblePointerStart, TextBox::SelectionStart - TextBox::VisiblePointerStart), "Verdana", 11).x;
			TextBox::SelectingEndPosition = GetTextSize(TextBox::VisibleString.substr(TextBox::VisiblePointerStart, TextBox::SelectionEnd - TextBox::VisiblePointerStart), "Verdana", 11).x;
		}
		else
		{
			TextBox::SelectedPosition = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::SelectedPoint - TextBox::VisiblePointerStart), "Verdana", 11).x;
			TextBox::SelectingStartPosition = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::SelectionStart - TextBox::VisiblePointerStart), "Verdana", 11).x;
			TextBox::SelectingEndPosition = GetTextSize(MainString->substr(TextBox::VisiblePointerStart, TextBox::SelectionEnd - TextBox::VisiblePointerStart), "Verdana", 11).x;
		}
	}
}

void TextBox::Draw()
{
	if (!TextBox::Parent)
		TextBox::SetVisible(false);
	if (!TextBox::IsVisible())
		return;

	MyColour rectColour = MenuColours["TextBox"];
	MyColour rectOutlineColour = MenuColours["TextBoxOutline"];
	MyColour textColour = MenuColours["Text"];
	MyColour highlightColour = MenuColours["TextBoxHighlight"];
	MyColour currentLocColour = MenuColours["TextBoxCurrent"];
	MyColour contextOutlineColour = MenuColours["TextBoxContextOutline"];
	MyColour contextLineColour = MenuColours["TextBoxContextFirstLine"];

	FilledRoundedRectangle(TextBox::Pos.x + TextBox::ParentPos.x - 1, TextBox::Pos.y + +TextBox::ParentPos.y - 1, TextBox::Size.x + 2, TextBox::Size.y + 2, 4, rectOutlineColour);
	FilledRoundedRectangle(TextBox::Pos.x + TextBox::ParentPos.x, TextBox::Pos.y + +TextBox::ParentPos.y, TextBox::Size.x, TextBox::Size.y, 4, rectColour);
	DrawText(TextBox::ParentPos.x + TextBox::Pos.x, TextBox::ParentPos.y + TextBox::Pos.y - (TextBox::Size.y / 1.5) - 1, TextBox::Name + L":", "Verdana", 11, textColour, None); // Title
	if (TextBox::hide_text)
		std::fill(TextBox::VisibleString.begin(), TextBox::VisibleString.end(), L'*');
	DrawText(TextBox::ParentPos.x + TextBox::Pos.x + 3, (TextBox::ParentPos.y + TextBox::Pos.y) + (TextBox::Size.y / 6), TextBox::VisibleString, "Verdana", 11, textColour, None);

	std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - TextBox::AnimationStart;
	float time = std::fmodf(elapsed.count(), TextBox::AnimationInterval) / TextBox::AnimationInterval;
	float easedtime = InOutSine(time);
	if (TextBox::Active && std::fmod(elapsed.count(), TextBox::AnimationInterval) < TextBox::AnimationInterval / 2)
	{
		float alpha = 255.0f * (1.0f - easedtime * 2.0f);
		FilledLine(TextBox::Pos.x + TextBox::ParentPos.x + TextBox::SelectedPosition + 5.f, TextBox::Pos.y + TextBox::ParentPos.y + TextBox::Size.y - 3, TextBox::Pos.x + TextBox::ParentPos.x + TextBox::SelectedPosition + 5.f, TextBox::Pos.y + TextBox::ParentPos.y + 3, 1,
			currentLocColour.Modify(currentLocColour.r, currentLocColour.g, currentLocColour.b, static_cast<float>(alpha) / 255.0f));
	}
	if (TextBox::SelectingStartPosition >= 0 || TextBox::SelectingEndPosition >= 0)
	{
		float selectionWidth = std::abs(TextBox::SelectingEndPosition - TextBox::SelectingStartPosition);
		float startX = TextBox::Pos.x + TextBox::ParentPos.x + TextBox::SelectingStartPosition;
		// Check the direction of selection
		if (TextBox::SelectingEndPosition > TextBox::SelectingStartPosition)
			startX += 5.f;
		else
			startX -= 5.f;
		FilledRectangle(startX, TextBox::Pos.y + TextBox::ParentPos.y, selectionWidth, TextBox::Size.y, highlightColour);
	}
	if (TextBox::ContextActive)
	{
		OutlineRectangle(TextBox::ContextPos.x, TextBox::ContextPos.y, TextBox::ContextSize.x, TextBox::ContextSize.y, 1, textColour);
		FilledRectangle(TextBox::ContextPos.x, TextBox::ContextPos.y, TextBox::ContextSize.x, TextBox::ContextSize.y, rectColour);
		int i = 0;
		for (auto pair : TextBox::ContextNames)
		{
			if (i != 0)
				FilledLine(TextBox::ContextPos.x, TextBox::ContextPos.y + i * 20, TextBox::ContextPos.x + TextBox::ContextSize.x, TextBox::ContextPos.y + i * 20, 1.0f, contextLineColour);

			if (IsMouseInRectangle(TextBox::ContextPos.x, TextBox::ContextPos.y + (i * 20), TextBox::ContextSize.x, 20))
				FilledRectangle(TextBox::ContextPos.x, TextBox::ContextPos.y + (i * 20), TextBox::ContextSize.x, 20, contextOutlineColour);
			DrawText(TextBox::ContextPos.x + (TextBox::ContextSize.x / 2), TextBox::ContextPos.y + (i * 20) + 10, pair.first, "Verdana", 11, textColour, CentreCentre);

			i++;
		}
	}
}
