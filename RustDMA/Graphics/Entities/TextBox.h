#pragma once
#include "Entity.h"

class TextBox : public Entity
{
protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> AnimationStart;
	float AnimationInterval = 1.0f;

	std::chrono::time_point<std::chrono::high_resolution_clock> SelectingAnimationStart;
	float SelectingAnimationInterval = 0.005f;

	int SelectedPoint;
	float SelectedPosition;

	int SelectionStart;
	int SelectionEnd;
	float SelectingStartPosition = 0;
	float SelectingEndPosition = 0;

	int VisiblePointerStart;
	int VisiblePointerEnd;
	void SetStartIndex();

	bool hide_text = false;

	std::wstring* MainString;
	std::wstring VisibleString;

	float TextWidth = 0;

	bool ContextActive = false;
	Vector2 ContextPos;
	Vector2 ContextSize;
	void ContextCopyText();
	void ContextSelectAll();
	void ContextPasteText();
	std::map<std::wstring, std::function<void()>> ContextNames = {
		{L"Select All", [this]() { ContextSelectAll(); }},
		{L"Copy", [this]() { ContextCopyText(); }},
		{L"Paste", [this]() { ContextPasteText(); }}
	};

	bool Active = false;
	bool Selecting = false;
	bool Held = false;

	bool IsKeyAcceptable();

	void SetState();
	void ArrowKeyNavition();
	void InputText();
	void DeleteText();
	void SetSelectionPoint();
	void SelectionDragging();
	void CopyText();
	void PasteText();
	void ContextMenu();
	void SetSelection();

public:
	TextBox(float x, float y, std::wstring text, std::wstring* data, bool hide = false);
	void Update();
	void Draw();
	bool IsTabbed;
};
