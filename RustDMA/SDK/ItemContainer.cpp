#include "Pch.h"
#include "Globals.h"
#include "ItemContainer.h"
#include "PlayerInventory.h"
ItemContainer::ItemContainer(uint64_t address)
{
	this->Class = address;
	printf("[ItemContainer] Initialized\n");
	this->ItemList = TargetProcess.Read<uint64_t>(Class + ItemList);
	printf("[ItemContainer] ItemList: 0x%llX\n", ItemList);
}

uint64_t ItemContainer::GetItemList()
{
	return ItemList;
}
Item* ItemContainer::GetItemBySlot(uint64_t slot)
{
	return new Item(TargetProcess.Read<uint64_t>(ItemList + 0x20 + (slot * 0x8)));
}