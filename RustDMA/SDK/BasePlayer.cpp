#include "Pch.h"
#include "Globals.h"
#include "BasePlayer.h"

BasePlayer::BasePlayer(uint64_t address)
{
	this->Class = address;
	printf("[BasePlayer] Initialized\n");
	auto handle = TargetProcess.CreateScatterHandle();
	TargetProcess.QueueScatterReadEx(handle,Class + BaseMovementOffset, reinterpret_cast<void*>(&BaseMovementOffset),sizeof(uint64_t));
	TargetProcess.QueueScatterReadEx(handle, Class + ActiveItemIDOffset, reinterpret_cast<void*>(&ActiveItemID), sizeof(uint32_t));
	TargetProcess.QueueScatterReadEx(handle, Class + PlayerInventory, reinterpret_cast<void*>(&PlayerInventory), sizeof(uint64_t));
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	printf("[BasePlayer] BaseMovement: 0x%llX\n", BaseMovementOffset);
	printf("[BasePlayer] ActiveItemID: 0x%llX\n", ActiveItemID);
//	printf("[BasePlayer] PlayerInventory: 0x%llX\n", PlayerInventoryOffset);
	this->BaseMovementInstance = new BaseMovement(BaseMovementOffset);
	ContainerBelt = TargetProcess.Read<uint64_t>(PlayerInventory + ContainerBelt);
}
BasePlayer::~BasePlayer()
{
	delete BaseMovementInstance;
}
PlayerFlags BasePlayer::GetPlayerFlag()
{
	if (!IsPlayerValid())
		return PlayerFlags::Connected;
	PlayerFlags flag = TargetProcess.Read<PlayerFlags>(Class + PlayerFlag);
	printf("[BasePlayer] PlayerFlag: %d\n", flag);
	return flag;
}
void BasePlayer::WritePlayerFlag(VMMDLL_SCATTER_HANDLE handle, PlayerFlags flag)
{
	if (!IsPlayerValid())
		return;
	if(!TargetProcess.QueueScatterWriteEx<PlayerFlags>(handle,Class + PlayerFlag,flag))
			printf("[BasePlayer] Failed to write PlayerFlag\n");
}
uint32_t BasePlayer::GetActiveItemID()
{
	return ActiveItemID;
}
// call this in the local player loop to keep the value updated as it changes depending on the item
void BasePlayer::UpdateActiveItemID(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.QueueScatterReadEx(handle, Class + ActiveItemIDOffset, reinterpret_cast<void*>(&ActiveItemID), sizeof(uint64_t));
}
BaseMovement* BasePlayer::GetBaseMovement()
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
	TargetProcess.QueueScatterReadEx(handle, itemlist + ItemListContents, reinterpret_cast<void*>(&items), sizeof(uint64_t));
	uint32_t itemsize = 0;
	TargetProcess.QueueScatterReadEx(handle, itemlist + ItemListSize, reinterpret_cast<void*>(&itemsize), sizeof(uint32_t));
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	BeltContainerList.resize(itemsize);

	std::vector<uint64_t> objectpointrs;
	objectpointrs.resize(itemsize);


	handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < itemsize; i++)
	{
		TargetProcess.QueueScatterReadEx(handle, items + 0x20 + (i * 0x8), reinterpret_cast<void*>(&objectpointrs[i]), sizeof(uint64_t));
	}
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	for (int i = 0; i < itemsize; i++)
	{
		if (objectpointrs[i] == NULL)
			continue;
		BeltContainerList.push_back(new Item(objectpointrs[i]));
	}
}
// it appears that we cant get the item class correctly, no idea where the issue lies. 
Item* BasePlayer::GetActiveItem()
{
	if (ActiveItemID == 0)
		return nullptr;
	if (!IsPlayerValid())
		return nullptr;
	Item* founditem = nullptr;
	for (Item* item : BeltContainerList)
	{
		if (item == NULL)
			continue; // no wasting reads and writes on null pointers

		int activeweaponid = item->GetItemID();
	
		if (ActiveItemID == activeweaponid)
		{

			printf("Found Item ID: %d\n", activeweaponid);
			founditem =  item;
		}
		else
		{
			printf("None Active Weapon ID: %d\n", activeweaponid);
			delete item; // DISPOSE OF THE WASTE!!!
		}
		
	}
	return founditem;
	
}

bool BasePlayer::IsPlayerValid()
{
	return Class != 0 && BaseMovementOffset != 0 && PlayerInventory !=0;
}