#include "pch.h"
#include "Input.h"
#include "Init.h"
// local scope enum
enum class KeyState
{
	KeyDormant = 0,
	KeyInvoked = 1,
	KeyActive = 2
};

// could do dictionaries but we only want to handle ascii
std::map<int, bool> KeyHeld;
std::map<int, KeyState> KeyStates;
std::map<int, float> KeyTimes;

Vector2 MousePos;
WPARAM Char = NULL;
HCURSOR CurrentCursor;
std::map<std::string, HCURSOR> Cursors;

void CreateCursor(std::string name, HCURSOR cursor)
{
	Cursors[name] = cursor;
}

void SetCurrentCursor(std::string name)
{
	CurrentCursor = Cursors[name];
}

HCURSOR GetCurrentCursor()
{
	return CurrentCursor;
}

void UpdateKeyState(int key, bool down)
{
	if (key < 0 || key >= KeyStates.size())
		return;
	KeyTimes[key] = clock() * 0.001f;
	if (KeyHeld[key] && !down)
		KeyStates[key] = KeyState::KeyDormant;

	if (KeyHeld[key] && down)
		KeyStates[key] = KeyState::KeyActive;

	if (!KeyHeld[key] && down)
		KeyStates[key] = KeyState::KeyInvoked;

	KeyHeld[key] = down;
}

LRESULT CALLBACK InputWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOVE:
		SetWindowPos(hWnd, NULL, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_NOACTIVATE);
		InitD2D(hWnd);
		break;
	case WM_CHAR:
		Char = wParam;
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		UpdateKeyState(wParam & 0xFF, false);
	}
	break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		UpdateKeyState(wParam & 0xFF, true);
	}
	break;
	case WM_LBUTTONUP:
	{
		UpdateKeyState(VK_LBUTTON, false);
	}
	break;

	case WM_MBUTTONUP:
	{
		UpdateKeyState(VK_MBUTTON, false);
	}
	break;

	case WM_RBUTTONUP:
	{
		UpdateKeyState(VK_RBUTTON, false);
	}
	break;
	case WM_MOUSEMOVE:
	{
		//https://stackoverflow.com/questions/21129721/how-does-this-bitmask-apply-to-lparam-wm-hotkey
		// prevents calling getcursorpos
		MousePos.x = static_cast<float>(lParam & 0xFFFF);
		MousePos.y = static_cast<float>((lParam >> 16) & 0xFFFF);
	}
	break;
	case WM_XBUTTONUP:
	{
		const auto releasedkey = (wParam >> 16) & 0xFFFF;

		if (releasedkey == XBUTTON1)
			UpdateKeyState(VK_XBUTTON1, false);
		else if (releasedkey == XBUTTON2)
			UpdateKeyState(VK_XBUTTON2, false);
	}
	break;
	case WM_XBUTTONDOWN:
	{
		const auto releasedkey = (wParam >> 16) & 0xFFFF;

		if (releasedkey == XBUTTON1)
			UpdateKeyState(VK_XBUTTON1, true);
		else if (releasedkey == XBUTTON2)
			UpdateKeyState(VK_XBUTTON2, true);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		UpdateKeyState(VK_LBUTTON, true);
	}
	break;
	case WM_MBUTTONDOWN:
	{
		UpdateKeyState(VK_MBUTTON, true);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		UpdateKeyState(VK_RBUTTON, true);
	}
	break;

	return 0;
	}
}

// check for a held key
bool IsKeyDown(int key)
{
	if (key < 0 || key >= KeyStates.size())
		return false;
	if (KeyHeld[key] == true)
	{
		return true;
	}
	else
		return false;
}

// check for a clicked key
bool IsKeyClicked(int key)
{
	if (key < 0 || key >= KeyStates.size())
		return false;
	// Check if the click time is below the current time by 0.05 seconds and is invoked, Kinda messy but works well.
	if (KeyStates[key] == KeyState::KeyInvoked && (KeyTimes[key]) >= (clock() * 0.001f) - 0.05f)
		return true;
	else
		return false;
}

// just fill the dictionaries
void SetInput()
{
	for (int i = 0; i <= 255; i++) // 255 is for the ascii character set, expand it for utf8 unicode
	{
		KeyStates[i] = KeyState::KeyDormant;
		KeyHeld[i] = false;
	}
}

bool IsMouseInRectangle(int x, int y, int width, int height)
{
	return (
		MousePos.x > x &&
		MousePos.y > y &&
		MousePos.x < x + width &&
		MousePos.y < y + height
		);
}

bool IsMouseInRectangle(Vector2 pos, Vector2 Dimensions)
{
	return (
		MousePos.x > pos.x &&
		MousePos.y > pos.y &&
		MousePos.x < pos.x + Dimensions.x &&
		MousePos.y < pos.y + Dimensions.y
		);
}

float TriangleArea(Vector2 point1, Vector2 point2, Vector2 point3)
{
	return (point1.x - point3.x) * (point2.y - point3.y) - (point2.x - point3.x) * (point1.y - point3.y);
}

bool IsMouseInTriangle(Vector2 point1, Vector2 point2, Vector2 point3)
{
	float d1;
	float d2;
	float d3;

	bool neg;
	bool pos;

	d1 = TriangleArea(MousePos, point1, point2);
	d2 = TriangleArea(MousePos, point2, point3);
	d3 = TriangleArea(MousePos, point3, point1);

	neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(neg && pos);
}
