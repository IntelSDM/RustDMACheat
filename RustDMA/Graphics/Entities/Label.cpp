#include "pch.h"
#include "Drawing.h"
#include "label.h"

Label::Label(std::wstring text, float posx, float posy)
{
	Label::Pos = {posx, posy};
	Label::Name = text;
	SetVisible(true);
}

void Label::Update()
{
	if (!Label::Parent)
		Label::SetVisible(false);
	if (!Label::IsVisible())
		return;
	Label::ParentPos = Label::Parent->GetParentPos();
}

void Label::Draw()
{
	if (!Label::Parent)
		Label::SetVisible(false);
	if (!Label::IsVisible())
		return;

	MyColour textColour = MenuColours[LIT("Text")];

	DrawText(Label::ParentPos.x + Label::Pos.x, Label::ParentPos.y + Label::Pos.y, Label::Name, LIT("Verdana"), 12, textColour, None);
}
