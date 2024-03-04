#include "Pch.h"
#include "TabListBox.h"
#include "GUI.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"
#include "Graphics.h"

TabListBox::TabListBox(std::wstring name)
{
	TabListBox::Name = name;
	TabListBox::Index = TabCount;
	TabCount++;
	SetVisible(true);
}

void TabListBox::Update()
{
	TabListBox::ParentPos = TabListBox::Parent->GetParentPos();
	TabListBox::Pos = TabListBox::Parent->GetParentPos();
	Container::Update();
}

void TabListBox::Draw()
{
	Container::Draw();
}
