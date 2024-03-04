#include "Pch.h"
#include "Globals.h"
#include "BaseProjectile.h"

// these are universal
std::map<uint32_t, float> OriginalRecoilPitchMin;
std::map<uint32_t, float> OriginalRecoilPitchMax;
std::map<uint32_t, float> OriginalRecoilYawMin;
std::map<uint32_t, float> OriginalRecoilYawMax;
std::map<uint32_t,bool> RejectedItems;
BaseProjectile::BaseProjectile(uint64_t address)
{
//	printf("[BaseProjectile] Initialized\n");
	this->Class = address;
	//printf("[BaseProjectile] Class: 0x%llX\n", Class);
	this->RecoilProperties = TargetProcess.Read<uint64_t>(Class + RecoilProperties);
	if (RecoilProperties == 0)
		return;
	//printf("[BaseProjectile] RecoilProperties: 0x%llX\n", RecoilProperties);
	if (IsValidWeapon())
	{
		uint64_t recoiloverride = TargetProcess.Read<uint64_t>(RecoilProperties + RecoilOverride);
		if (recoiloverride == 0)
			RecoilOverride = RecoilProperties; // some guns don't have a new recoil pattern and use an old one. which is located at recoilproperties instead of override
		else
			RecoilOverride = recoiloverride;
	}
}
bool BaseProjectile::IsValidWeapon()
{
	return RecoilProperties != 0;
}

void BaseProjectile::WriteRecoilYaw(VMMDLL_SCATTER_HANDLE handle, uint32_t itemid, int percent)
{
	if (OriginalRecoilYawMin.find(itemid) == OriginalRecoilYawMin.end() && !RejectedItems.contains(itemid)) // save on reads 
	{
		OriginalRecoilYawMin[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilYawMin);
		OriginalRecoilYawMax[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilYawMax);
		printf("[BaseProjectile] RecoilYawMin: %f\n", OriginalRecoilYawMin[itemid]);
		printf("[BaseProjectile] RecoilYawMax: %f\n", OriginalRecoilYawMax[itemid]);
		if (OriginalRecoilYawMin[itemid] == 0 && OriginalRecoilYawMax[itemid] == 0)
		{
			RejectedItems[itemid] = true;
			printf("[BaseProjectile] Rejected ItemID: %d\n", itemid);
			return;
		}
	}
	if (RejectedItems.contains(itemid))
	{
		return;
	}
	float yawmin = OriginalRecoilYawMin[itemid];
	float yawmax = OriginalRecoilYawMax[itemid];
	float yawminpercent = yawmin * (percent / 100.0f);
	float yawmaxpercent = yawmax * (percent / 100.0f);
	TargetProcess.AddScatterWriteRequest<float>(handle, RecoilOverride + RecoilYawMin, yawminpercent);
	TargetProcess.AddScatterWriteRequest<float>(handle,RecoilOverride + RecoilYawMax, yawmaxpercent);
}

void BaseProjectile::WriteRecoilPitch(VMMDLL_SCATTER_HANDLE handle, uint32_t itemid, int percent)
{
	if (OriginalRecoilPitchMin.find(itemid) == OriginalRecoilPitchMin.end() && !RejectedItems.contains(itemid))
	{
		OriginalRecoilPitchMin[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilPitchMin);
		OriginalRecoilPitchMax[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilPitchMax);
		printf("[BaseProjectile] RecoilPitchMin: %f\n", OriginalRecoilPitchMin[itemid]);
		printf("[BaseProjectile] RecoilPitchMax: %f\n", OriginalRecoilPitchMax[itemid]);
		if (OriginalRecoilPitchMin[itemid] == 0 && OriginalRecoilPitchMax[itemid] == 0)
		{
			RejectedItems[itemid] = true;
			printf("[BaseProjectile] Rejected ItemID: %d\n", itemid);
			return;
		}
	}
	if (RejectedItems.contains(itemid))
	{
		return;
	}
	float pitchmin = OriginalRecoilPitchMin[itemid];
	float pitchmax = OriginalRecoilPitchMax[itemid];
	float pitchminpercent = pitchmin * (percent / 100.0f);
	float pitchmaxpercent = pitchmax * (percent / 100.0f);		
	TargetProcess.AddScatterWriteRequest<float>(handle, RecoilOverride + RecoilPitchMin, pitchminpercent);
	TargetProcess.AddScatterWriteRequest<float>(handle, RecoilOverride + RecoilPitchMax, pitchmaxpercent);
}
