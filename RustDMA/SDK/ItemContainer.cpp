#include "Pch.h"
#include "Globals.h"
#include "ItemContainer.h"
#include "PlayerInventory.h"
ItemContainer::ItemContainer(uint64_t address)
{
	if (address == 0)
	{
		printf("[ItemContainer] Failed to initialize\n");
		return;
	}
	this->Class = address;
	printf("[ItemContainer] Initialized\n");
	this->ItemList = TargetProcess.Read<uint64_t>(Class + ItemList);
	printf("[ItemContainer] ItemList: 0x%llX\n", ItemList);
}

uint64_t ItemContainer::GetItemList()
{
	return ItemList;
}
std::vector<Item*> ItemContainer::GetItemSlots()
{
	if(Class == 0 || ItemList==0)
		return std::vector<Item*>();
	std::vector<Item*> items;
	std::vector<uintptr_t> objectpointers;
	objectpointers.resize(6);
	auto handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < 6; i++) // size of items you can have in hotbar
	{
		TargetProcess.QueueScatterReadEx(handle, ItemList + 0x20 + (i * 0x8), reinterpret_cast<void*>(&objectpointers[i]), sizeof(uint64_t));
	}
	TargetProcess.ExecuteScatterRead(handle);
	for (int i = 0; i < 6; i++)
	{
		if (objectpointers[i] == NULL)
			items.push_back(nullptr);
		else
			items.push_back(new Item(objectpointers[i]));
	}

	return items;
}