#pragma once
#include "ItemContainer.h"
class PlayerInventory
{
	uint64_t Class = 0x0;
	uint64_t ContainerBelt = 0x28; // 	public global::ItemContainer containerBelt;
	ItemContainer* ContainerBeltInstance = nullptr;
public:
	PlayerInventory(uint64_t address);
	ItemContainer* GetItemContainer();
};