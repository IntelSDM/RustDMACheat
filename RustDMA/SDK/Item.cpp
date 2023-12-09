#include "pch.h"
#include "Globals.h"
#include "Item.h"

Item::Item(uint64_t address)
{
	Class = address;
	auto handle = TargetProcess.CreateScatterHandle();
	TargetProcess.QueueScatterReadEx(handle, Class + ItemID,reinterpret_cast<void*>(&ItemID),sizeof(uint64_t));
	TargetProcess.QueueScatterReadEx(handle, Class + HeldEntity, reinterpret_cast<void*>(&HeldEntity), sizeof(uint64_t));
	TargetProcess.ExecuteScatterRead(handle);
}

uint64_t Item::GetItemID()
{
	return ItemID;
}