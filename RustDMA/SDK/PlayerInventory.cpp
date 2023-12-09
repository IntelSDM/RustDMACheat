#include "Pch.h"
#include "PlayerInventory.h"
#include "Globals.h"

PlayerInventory::PlayerInventory(uint64_t address)
{
	this->Class = address;
	printf("[PlayerInventory] Initialized\n");
	this->ContainerBelt = TargetProcess.Read<uint64_t>(Class + ContainerBelt);
	printf("[PlayerInventory] ContainerBelt: 0x%llX\n", ContainerBelt);
	ContainerBeltInstance = new ItemContainer(ContainerBelt);
}
ItemContainer* PlayerInventory::GetItemContainer()
{
	return ContainerBeltInstance;
}