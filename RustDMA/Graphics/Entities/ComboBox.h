#pragma once
#include "Entity.h"

class ComboBox : public Entity
{
protected:
	float DropWidth;
	std::list<std::wstring> Names;
	std::list<bool*> Items;
	bool Active = false;
	std::wstring SelectedName;
	float TextWidth = 0;
	float CutOffBuffer = 0;
	float SizeDifference;

	int MaxVisibleItems = 5;

	int PointerStart = 0;
	int PointerEnd = MaxVisibleItems;

	void ConvertSelectedName();
	void CalculateBuffer();
	void SetComboBoxWidth();
	void ArrowNavigation();
	void UpdateScrollBar();

	bool ScrollBarHeld = false;

public:
	ComboBox(float x, float y, std::wstring text, std::list<bool*> items, std::list<std::wstring> names);
	void Update();
	void Draw();
};
