#include "Pch.h"
#include "Globals.h"
#include "BasePlayer.h"

BasePlayer::BasePlayer(uint64_t address,VMMDLL_SCATTER_HANDLE handle)
{
	this->Class = address;
	if (address == 0)
		return; // invalid
	
	TargetProcess.AddScatterReadRequest(handle,Class + BaseMovementOffset, reinterpret_cast<void*>(&BaseMovementOffset),sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, Class + ActiveItemIDOffset, reinterpret_cast<void*>(&ActiveItemID), sizeof(uint32_t));
	TargetProcess.AddScatterReadRequest(handle, Class + PlayerInventory, reinterpret_cast<void*>(&PlayerInventory), sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, Class + PlayerModel, reinterpret_cast<void*>(&PlayerModel), sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, Class + DisplayName, reinterpret_cast<void*>(&DisplayName), sizeof(uint64_t));

}
void BasePlayer::InitializePlayerList()
{
	uint64_t staticclass = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(LIT("GameAssembly.dll")) + StaticClass);
	uint64_t staticfield = TargetProcess.Read<uint64_t>(staticclass + 0xb8); // access static fields
	uint64_t playerlist = TargetProcess.Read<uint64_t>(staticfield + VisiblePlayerList);
	VisiblePlayerList = TargetProcess.Read<uint64_t>(playerlist + 0x28);

}
int BasePlayer::GetPlayerListSize()
{
	return PlayerListSize;
}
void BasePlayer::CacheStage1(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerModel + Position, reinterpret_cast<void*>(&TransformPosition), sizeof(Vector3));
	TargetProcess.AddScatterReadRequest(handle, DisplayName + 0x14, reinterpret_cast<void*>(&PlayerName), sizeof(PlayerName));
	TargetProcess.AddScatterReadRequest(handle, PlayerModel + IsNPCOffset, reinterpret_cast<void*>(&NPC), sizeof(bool));
	TargetProcess.AddScatterReadRequest(handle, PlayerInventory + ContainerBelt, reinterpret_cast<void*>(&ContainerBelt),sizeof(uint64_t));
	this->BaseMovementInstance = std::make_shared<BaseMovement>(BaseMovementOffset);

	
}
void BasePlayer::CachePlayers()
{
	if (VisiblePlayerList == 0)
	{
		PlayerListSize = 0;
		PlayerList.clear();
		return;
	}
	std::vector< std::shared_ptr<BasePlayer>> templayerlist;
	auto handle = TargetProcess.CreateScatterHandle();
	uint32_t size;
	TargetProcess.AddScatterReadRequest(handle, VisiblePlayerList + 0x10, reinterpret_cast<void*>(&size), sizeof(uint32_t));
	uint64_t buffer;
	TargetProcess.AddScatterReadRequest(handle, VisiblePlayerList + 0x18, reinterpret_cast<void*>(&buffer), sizeof(uint64_t));
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);

	PlayerListSize = size;
	if (size == 0 || buffer == 0)
		return;
	std::vector<uint64_t> playerlist;
	playerlist.resize(size);
	handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < size; i++)
	{
		TargetProcess.AddScatterReadRequest(handle, buffer + (0x20 + (i * 8)), reinterpret_cast<void*>(&playerlist[i]), sizeof(uint64_t));
	}
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);

	handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < size; i++)
	{
		if (playerlist[i] == NULL)
			continue;
		templayerlist.push_back(std::make_shared<BasePlayer>(playerlist[i], handle));

	}
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);

	handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < templayerlist.size(); i++)
	{
		std::shared_ptr<BasePlayer> player = templayerlist[i];
		player->CacheStage1(handle);
		
	}
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	PlayerList = templayerlist;
}
BasePlayer::~BasePlayer()
{
}
PlayerFlags BasePlayer::GetPlayerFlag()
{
	if (!IsPlayerValid())
		return PlayerFlags::Connected;
	PlayerFlags flag = TargetProcess.Read<PlayerFlags>(Class + PlayerFlag);
	return flag;
}
void BasePlayer::WritePlayerFlag(PlayerFlags flag)
{
	if (!IsPlayerValid())
		return;
	if(!TargetProcess.Write<PlayerFlags>(Class + PlayerFlag,flag))
			printf("[BasePlayer] Failed to write PlayerFlag\n");
}
uint32_t BasePlayer::GetActiveItemID()
{
	return ActiveItemID;
}
// call this in the local player loop to keep the value updated as it changes depending on the item
void BasePlayer::UpdateActiveItemID(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle,Class + ActiveItemIDOffset, reinterpret_cast<void*>(&ActiveItemID), sizeof(uint64_t));
}
std::shared_ptr<BaseMovement> BasePlayer::GetBaseMovement()
{
	return BaseMovementInstance;
}
void BasePlayer::SetupBeltContainerList()
{
	if (!IsPlayerValid())
		return;
	BeltContainerList.clear();
	uint64_t itemlist = TargetProcess.Read<uint64_t>(ContainerBelt + ItemList); // yeah you need to reread this constantly, if you don't hell breaks loose. 
	auto handle = TargetProcess.CreateScatterHandle();
	uint64_t items = 0;
	TargetProcess.AddScatterReadRequest(handle, itemlist + ItemListContents, reinterpret_cast<void*>(&items), sizeof(uint64_t));
	uint32_t itemsize = 0;
	TargetProcess.AddScatterReadRequest(handle, itemlist + ItemListSize, reinterpret_cast<void*>(&itemsize), sizeof(uint32_t));
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	BeltContainerList.resize(itemsize);	

	std::vector<uint64_t> objectpointrs;
	objectpointrs.resize(itemsize);


	handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < itemsize; i++)
	{
		TargetProcess.AddScatterReadRequest(handle, items + 0x20 + (i * 0x8), reinterpret_cast<void*>(&objectpointrs[i]), sizeof(uint64_t));
	}
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	for (int i = 0; i < itemsize; i++)
	{
		if (objectpointrs[i] == NULL)
			continue;
		BeltContainerList.push_back(std::make_shared<Item>(objectpointrs[i]));
	}
}
// it appears that we cant get the item class correctly, no idea where the issue lies. 
std::shared_ptr<Item> BasePlayer::GetActiveItem()
{
	if (ActiveItemID == 0)
		return nullptr;
	if (!IsPlayerValid())
		return nullptr;
	std::shared_ptr<Item> founditem = nullptr;
	for (std::shared_ptr<Item> item : BeltContainerList)
	{
		if (item == NULL)
			continue; // no wasting reads and writes on null pointers

		int activeweaponid = item->GetItemID();

		if (ActiveItemID == activeweaponid)
		{

			founditem =  item;
			break;
		}
		
	}
	return founditem;
	
}

bool BasePlayer::IsPlayerValid()
{
	return Class != 0 && PlayerInventory != 0;
}

bool BasePlayer::IsSleeping()
{
	if (!IsPlayerValid())
		return false;
	return (ActiveFlag & (int)16) == (int)16;
}
bool BasePlayer::IsNPC()
{
	if (!IsPlayerValid())
		return false;
	return NPC;
}

std::wstring BasePlayer::GetName()
{
	if(!IsNPC())
	return std::wstring(PlayerName);
	else
		return std::wstring(LIT(L"Scientist"));
}
std::vector<std::shared_ptr<BasePlayer>> BasePlayer::GetPlayerList()
{
	return PlayerList;
}
uint64_t BasePlayer::GetClass()
{
	return Class;
}
void BasePlayer::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerModel + Position, reinterpret_cast<void*>(&TransformPosition), sizeof(Vector3));
}
void BasePlayer::UpdateDestroyed(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, Class + DestroyedOffset, reinterpret_cast<void*>(&Destroyed), sizeof(bool));
}
Vector3 BasePlayer::GetPosition()
{
	return TransformPosition;
}
void BasePlayer::UpdateActiveFlag(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, Class + PlayerFlag, reinterpret_cast<void*>(&ActiveFlag), sizeof(int));
}
void BasePlayer::WriteActiveFlag(int flag)
{
	if (!IsPlayerValid())
		return;
	if (!TargetProcess.Write<int>(Class + PlayerFlag, flag))
		printf("[BasePlayer] Failed to write flag\n");
}
int BasePlayer::GetActiveFlag()
{
	return ActiveFlag;
}