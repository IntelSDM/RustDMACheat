#pragma once
#include "Entity.h"

class Label : public Entity
{
protected:
    bool Tabbed;
public:
    Label(std::wstring name, float x, float y);
    void Update();
    void Draw();
};

