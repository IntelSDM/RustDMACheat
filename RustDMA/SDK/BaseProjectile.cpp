#include "Pch.h"
#include "Globals.h"
#include "BaseProjectile.h"

BaseProjectile::BaseProjectile(uint64_t address)
{
	printf("[BaseProjectile] Initialized\n");
	this->Class = address;
	printf("[BaseProjectile] Class: 0x%llX\n", Class);
	this->RecoilProperties = TargetProcess.Read<uint64_t>(Class + RecoilProperties);
	printf("[BaseProjectile] RecoilProperties: 0x%llX\n", RecoilProperties);
	if (IsValidWeapon())
	{
		this->RecoilOverride = TargetProcess.Read<uint64_t>(RecoilProperties + RecoilOverride);
		if(!TargetProcess.Write<float>(RecoilOverride + RecoilPitchMax, 0.0f))
			printf("[BaseProjectile] Failed to write RecoilPitchMax\n");
		TargetProcess.Write<float>(RecoilOverride + RecoilYawMax, 0.0f);
		TargetProcess.Write<float>(RecoilOverride + RecoilYawMin, 0.0f);
		TargetProcess.Write<float>(RecoilOverride + RecoilPitchMin, 0.0f);
	}
}
bool BaseProjectile::IsValidWeapon()
{
	return RecoilProperties != 0;
}