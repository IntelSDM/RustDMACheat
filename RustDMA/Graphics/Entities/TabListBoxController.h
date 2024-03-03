#pragma once
#include "Entity.h"
#include "TabListBox.h"

class TabListBoxController : public Container
{
protected:
	std::list<std::shared_ptr<TabListBox>> Tabs;
	std::list<std::wstring> Names;
	std::list<std::wstring> CulledNames;
	int ScrollWidth = 5;
	int Selected;
	bool ScrollBarHeld = false;
	bool Active = false;

	void UpdateCulledNames();
	void ArrowKeyNavigation();
	void ScrollBarAction();
	void SetActiveIndex();
	void SetActive();
	int PointerStart = 2;
	int PointerEnd;
	int MaxVisibleItems = TabListBoxController::Size.y / 20;
	int ActiveIndex = 0;
	int CachedHeight;

	bool FirstItem = false;

public:
	TabListBoxController(float x, float y, float width, float height);
	void Update();
	void Draw();
	void PushBack(std::shared_ptr<TabListBox> tab);
	int GetActiveIndex();
};
