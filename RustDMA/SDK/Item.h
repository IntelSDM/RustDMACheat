#pragma once
#include "BaseProjectile.h"
class Item
{
	uint64_t Class = 0x0;
	uint64_t ItemID = 0x20; // public ItemId uid;
	uint64_t HeldEntity = 0xB0; //private EntityRef heldEntity;
	BaseProjectile* BaseProjectileInstance = nullptr;
public:
	Item(uint64_t address);
	~Item();
	uint64_t GetItemID();
	uint64_t GetHeldEntity();
	BaseProjectile* GetBaseProjectile();
};