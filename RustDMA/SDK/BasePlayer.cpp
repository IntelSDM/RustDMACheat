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
	this->PlayerInventoryInstance = new PlayerInventory(PlayerInventoryOffset);
}
BasePlayer::~BasePlayer()
{
	delete BaseMovementInstance;
	delete PlayerInventoryInstance;
}
PlayerFlags BasePlayer::GetPlayerFlag()
{
	PlayerFlags flag = TargetProcess.Read<PlayerFlags>(Class + PlayerFlag);
	printf("[BasePlayer] PlayerFlag: %d\n", flag);
	return flag;
}
void BasePlayer::WritePlayerFlag(PlayerFlags flag)
{
	if(!TargetProcess.Write<PlayerFlags>(Class + PlayerFlag,flag))
			printf("[BasePlayer] Failed to write PlayerFlag\n");
}
uint64_t BasePlayer::GetActiveItemID()
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

Item* BasePlayer::GetActiveItem()
{
	if (ActiveItemID == 0)
		return nullptr;
	printf("Active Item ID: %d\n", ActiveItemID);
	std::vector<uintptr_t> objectpointrs;
	objectpointrs.resize(6);
	uint64_t items = PlayerInventoryInstance->GetItemContainer()->GetItemList();
	auto handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < 6; i++)
	{
		TargetProcess.QueueScatterReadEx(handle, items + 0x20 + (i * 0x8), reinterpret_cast<void*>(&objectpointrs[i]), sizeof(uint64_t));
	}
	TargetProcess.ExecuteScatterRead(handle);
	TargetProcess.CloseScatterHandle(handle);
	Item* founditem = nullptr;
	for (int i = 0; i < 6; i++)
	{
		Item* item = new Item(objectpointrs[i]);
		int activeweaponid = item->GetItemID();
		printf("None Active Weapon ID: %d\n", item->GetItemID());
		if (ActiveItemID == activeweaponid)
		{

			printf("Found Item ID");
			founditem =  item;
		}
		delete item; // DISPOSE OF THE WASTE!!!
	}


	return founditem;
}

bool BasePlayer::IsPlayerValid()
{
	return Class != 0 && BaseMovementOffset !=0 && PlayerInventoryOffset !=0;
}