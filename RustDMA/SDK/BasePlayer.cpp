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
	TargetProcess.QueueScatterReadEx(handle, Class + PlayerInventoryOffset, reinterpret_cast<void*>(&PlayerInventoryOffset), sizeof(uint64_t));
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	printf("[BasePlayer] BaseMovement: 0x%llX\n", BaseMovementOffset);
	printf("[BasePlayer] ActiveItemID: 0x%llX\n", ActiveItemID);
	printf("[BasePlayer] PlayerInventory: 0x%llX\n", PlayerInventoryOffset);
	this->BaseMovementInstance = new BaseMovement(BaseMovementOffset);
}
BasePlayer::~BasePlayer()
{
	delete BaseMovementInstance;
}
PlayerFlags BasePlayer::GetPlayerFlag()
{
	PlayerFlags flag = TargetProcess.Read<PlayerFlags>(Class + PlayerFlag);
	printf("[BasePlayer] PlayerFlag: %d\n", flag);
	return flag;
}
void BasePlayer::WritePlayerFlag(VMMDLL_SCATTER_HANDLE handle, PlayerFlags flag)
{
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
// it appears that we cant get the item class correctly, no idea where the issue lies. 
Item* BasePlayer::GetActiveItem()
{
	if (ActiveItemID == 0)
		return nullptr;

	printf("Active Item ID: %d\n", ActiveItemID);
	std::vector<uint64_t> objectpointrs;
	objectpointrs.resize(6);
	
	uint64_t inventory = TargetProcess.Read<uint64_t>(Class + 0x8D8);
	uint64_t containerbelt = TargetProcess.Read<uint64_t>(inventory + 0x28);
	uint64_t itemlist = TargetProcess.Read<uint64_t>(containerbelt + 0x40);
	uint64_t items = TargetProcess.Read<uint64_t>(itemlist + 0x10);
	uint32_t itemsize = TargetProcess.Read<uint32_t>(itemlist + 0x18);
	auto handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < itemsize; i++)
	{
		TargetProcess.QueueScatterReadEx(handle, items + 0x20 + (i * 0x8), reinterpret_cast<void*>(&objectpointrs[i]), sizeof(uint64_t));
	}
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	Item* founditem = nullptr;
	for (int i = 0; i < itemsize; i++)
	{
		if (objectpointrs[i] == NULL)	
			continue; // no wasting reads and writes on null pointers
		printf("Item Pointer   0x%llX\n", objectpointrs[i]);
		Item* iteminstance = new Item(objectpointrs[i]);
		int activeweaponid = iteminstance->GetItemID();
		printf("Item ID: %d\n", activeweaponid);
	/*
		if (ActiveItemID == activeweaponid)
		{

			printf("Found Item ID: %d\n", activeweaponid);
		//	founditem =  item;
		}
		else
		{
			printf("None Active Weapon ID: %d\n", activeweaponid);
			//delete item; // DISPOSE OF THE WASTE!!!
		}*/
		
	}


	return founditem;
}

bool BasePlayer::IsPlayerValid()
{
	return Class != 0 && BaseMovementOffset !=0 && PlayerInventoryOffset !=0;
}