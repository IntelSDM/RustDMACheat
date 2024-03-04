#pragma once
#include "Entity.h"
class Form : public Container
{
protected:
	bool Dragging;
	bool Stretching;
	bool CanStretch;
	bool CanMinimize;
	bool CanClose;

	float InitTime;

	Vector2	Drag;
	Vector2	Stretch;
	Vector2 Border;
	Vector2 TitleBar;

	Vector2 Cursor;

	Vector2 StretchPoint1;
	Vector2 StretchPoint2;
	Vector2 StretchPoint3;

	void DragAction();
	void StretchAction();

public:
	Form(float x, float y, float w, float h, float borderwidth, float borderheight, std::wstring text, bool drag);
	void Draw();
	void Update();
};