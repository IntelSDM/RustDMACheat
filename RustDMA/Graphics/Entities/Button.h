#pragma once
#include "Entity.h"

class Button : public Entity
{
protected:
	std::function<void()> Action;
	void SetWidth();

public:
	Button(float x, float y, std::wstring text, std::function<void()> click);
	void SetSize(Vector2 size) { Button::Size = size; }
	void Update();
	void Draw();
};
