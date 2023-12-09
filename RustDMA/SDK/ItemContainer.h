#pragma once
#include "Item.h"
class ItemContainer
{

	uint64_t Class = 0x0;
	uint64_t ItemList = 0x40; // public List<global::Item> itemList;
public:
	ItemContainer(uint64_t container);
	uint64_t GetItemList();
	Item* GetItemBySlot(uint64_t slot);
};