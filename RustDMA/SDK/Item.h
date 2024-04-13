#pragma once
#include "BaseProjectile.h"
class Item
{
	uint64_t Class = 0x0;
	uint64_t Info = 0x18; // Item -> public ItemDefinition info;
	uint32_t ItemID = 0x20; // Item -> public ItemId uid;
	uint64_t HeldEntity = 0xB0; // Item -> private EntityRef heldEntity;
	std::shared_ptr<BaseProjectile> BaseProjectileInstance = nullptr;
public:
	Item(uint64_t address);
	~Item();
	uint32_t GetItemID();
	uint64_t GetHeldEntity();
	std::shared_ptr<BaseProjectile> GetBaseProjectile();
};
