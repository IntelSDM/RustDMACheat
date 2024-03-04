#pragma once
#include "Entity.h"
class DropDown : public Entity
{
protected:
    float DropWidth;
    std::vector<std::wstring>Names;
    int* Index;
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
    void SetDropDownWidth();
    void ArrowNavigation();
    void UpdateScrollBar();

    bool ScrollBar = false;


public:
    DropDown(float x, float y, std::wstring text, int* items, std::vector<std::wstring>names);
    void Update();
    void Draw();
};
