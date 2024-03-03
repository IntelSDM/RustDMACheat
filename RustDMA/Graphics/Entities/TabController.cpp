#include "pch.h"
#include "TabController.h"
#include "Drawing.h"
TabController::TabController()
{
	TabController::SetVisible(true);
}

void TabController::Update()
{
	if (!TabController::Parent)
		TabController::SetVisible(false);

	if (!TabController::IsVisible())
		return;
	TabController::ParentPos = TabController::Parent->GetParentPos();
	TabController::Pos.x = 0;
	TabController::Pos.y = 0;
	TabController::Size = {100, TabController::Parent->GetSize().y-30};
	TabController::Container::Update();
}

void TabController::Draw()
{
	if (!TabController::IsVisible())
		return;
	int size = TabController::Size.x / 12;

	TabController::Container::Draw();
}
