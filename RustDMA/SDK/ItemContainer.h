#pragma once
#include "Item.h"
class ItemContainer
{

	uint64_t Class = 0x0;
	uint64_t ItemList = 0x40; // public List<global::Item> itemList;
	uint64_t ItemListItems = 0x10; // don't worry about this pointer, its a static value for the item values in the list.
public:
	ItemContainer(uint64_t container);
	uint64_t GetItemList();
	std::vector<Item*> GetItemSlots();
};