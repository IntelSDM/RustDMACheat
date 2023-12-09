#include "Pch.h"
#include "Globals.h"
#include "BaseProjectile.h"

BaseProjectile::BaseProjectile(uint64_t address)
{
	printf("[BaseProjectile] Initialized\n");
	this->Class = address;
}