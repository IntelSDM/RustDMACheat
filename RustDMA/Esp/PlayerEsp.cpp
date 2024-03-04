#include "pch.h"
#include "globals.h"
#include "PlayerEsp.h"
#include "Visuals.h"
#include "Drawing.h"
#include "ConfigInstance.h"

std::shared_ptr<CheatFunction> UpdatePlayers = std::make_shared<CheatFunction>(20, []() {
	auto handle = TargetProcess.CreateScatterHandle();
	BaseLocalPlayer->UpdatePosition(handle);
	for (std::shared_ptr<BasePlayer> player : BaseLocalPlayer->GetPlayerList())
	{
		if (!player->IsPlayerValid())
			continue;
		if(player->GetClass() == BaseLocalPlayer->GetClass())
			continue;
		if (player->IsSleeping())
			continue;
		player->UpdatePosition(handle);
		player->UpdateDestroyed(handle);
		player->UpdateActiveFlag(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	});

void DrawPlayers()
{
	if(!ConfigInstance.PlayerESP.Enable)
				return;
	for (std::shared_ptr<BasePlayer> player : BaseLocalPlayer->GetPlayerList())
	{
		if (!player->IsPlayerValid())
			continue;
		if (player->GetClass() == BaseLocalPlayer->GetClass())
			continue;
		if (player->IsSleeping())
			continue;
	
		Vector3 position = player->GetPosition();
		if (position == Vector3(0, 0, 0))
			continue;
		Vector2 screenpos = WorldToScreen(position);
		if (screenpos.x == 0 && screenpos.y == 0)
			continue;
		
		int distance = (int)Vector3::Distance(position, BaseLocalPlayer->GetPosition());
		if(distance > ConfigInstance.PlayerESP.MaxDistance)
			continue;
		std::wstring name = ConfigInstance.PlayerESP.Name ? player->GetName() : LIT(L"");
		std::wstring distancestr = ConfigInstance.PlayerESP.Distance ? LIT(L"[") + std::to_wstring(distance) + LIT(L"m]") : LIT(L"");
		DrawText(screenpos.x, screenpos.y, name + distancestr, LIT("Verdana"), 11, ConfigInstance.PlayerESP.Colour, FontAlignment::Centre);


	}
}