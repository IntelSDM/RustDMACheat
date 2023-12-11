#include "Pch.h"
#include "Globals.h"
#include "BaseProjectile.h"

// these are universal
std::map<uint32_t, float> OriginalRecoilPitchMin;
std::map<uint32_t, float> OriginalRecoilPitchMax;
std::map<uint32_t, float> OriginalRecoilYawMin;
std::map<uint32_t, float> OriginalRecoilYawMax;

BaseProjectile::BaseProjectile(uint64_t address)
{
	printf("[BaseProjectile] Initialized\n");
	this->Class = address;
	printf("[BaseProjectile] Class: 0x%llX\n", Class);
	this->RecoilProperties = TargetProcess.Read<uint64_t>(Class + RecoilProperties);
	printf("[BaseProjectile] RecoilProperties: 0x%llX\n", RecoilProperties);
	if (IsValidWeapon())
	{
		uint64_t recoiloverride = TargetProcess.Read<uint64_t>(RecoilProperties + RecoilOverride);
		if (recoiloverride == 0)
			RecoilOverride = RecoilProperties; // some guns don't have a new recoil pattern and use an old one. which is located at recoilproperties instead of override
		else
			RecoilOverride = recoiloverride;
	//	if (!TargetProcess.Write<float>(RecoilOverride + RecoilPitchMax, 20 / 12 * 1.35))
	//		printf("[BaseProjectile] Failed to write RecoilPitchMax\n");
	//	TargetProcess.Write<float>(RecoilOverride + RecoilYawMax, 50 / 12 * 1.35);
	//	TargetProcess.Write<float>(RecoilOverride + RecoilYawMin, 50 / 12 * 1.35);
	//	TargetProcess.Write<float>(RecoilOverride + RecoilPitchMin,20/ 12 * 1.35);
	}
}
bool BaseProjectile::IsValidWeapon()
{
	return RecoilProperties != 0;
}

void BaseProjectile::WriteRecoilYaw(uint32_t itemid, int percent)
{
	if (OriginalRecoilYawMin.find(itemid) == OriginalRecoilYawMin.end())
	{
		OriginalRecoilYawMin[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilYawMin);
		OriginalRecoilYawMax[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilYawMax);
		printf("[BaseProjectile] RecoilYawMin: %f\n", OriginalRecoilYawMin[itemid]);
		printf("[BaseProjectile] RecoilYawMax: %f\n", OriginalRecoilYawMax[itemid]);
	}
	float yawmin = OriginalRecoilYawMin[itemid];
	float yawmax = OriginalRecoilYawMax[itemid];
	float yawminpercent = yawmin * (percent / 100.0f);
	float yawmaxpercent = yawmax * (percent / 100.0f);
	if (!TargetProcess.Write<float>(RecoilOverride + RecoilYawMin, yawminpercent))
		printf("[BaseProjectile] Failed to write RecoilYawMin\n");
	if (!TargetProcess.Write<float>(RecoilOverride + RecoilYawMax, yawmaxpercent))
		printf("[BaseProjectile] Failed to write RecoilYawMax\n");
}

void BaseProjectile::WriteRecoilPitch(uint32_t itemid, int percent)
{
	if (OriginalRecoilPitchMin.find(itemid) == OriginalRecoilPitchMin.end())
	{
		OriginalRecoilPitchMin[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilPitchMin);
		OriginalRecoilPitchMax[itemid] = TargetProcess.Read<float>(RecoilOverride + RecoilPitchMax);
		printf("[BaseProjectile] RecoilPitchMin: %f\n", OriginalRecoilPitchMin[itemid]);
		printf("[BaseProjectile] RecoilPitchMax: %f\n", OriginalRecoilPitchMax[itemid]);
	}
	float pitchmin = OriginalRecoilPitchMin[itemid];
	float pitchmax = OriginalRecoilPitchMax[itemid];
	float pitchminpercent = pitchmin * (percent / 100.0f);
	float pitchmaxpercent = pitchmax * (percent / 100.0f);		
	if (!TargetProcess.Write<float>(RecoilOverride + RecoilPitchMin, pitchminpercent))
		printf("[BaseProjectile] Failed to write RecoilPitchMin\n");
	if (!TargetProcess.Write<float>(RecoilOverride + RecoilPitchMax, pitchmaxpercent))
		printf("[BaseProjectile] Failed to write RecoilPitchMax\n");
}