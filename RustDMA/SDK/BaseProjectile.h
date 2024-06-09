#pragma once
class BaseProjectile
{
	uint64_t Class = 0x0;
	uint64_t RecoilProperties = 0x300; //	public RecoilProperties recoil;

	// Part of  RecoilProperties
	uint64_t RecoilOverride = 0x80; // RecoilProperties-> public RecoilProperties newRecoilOverride;
	uint64_t RecoilYawMin = 0x18; // RecoilProperties-> public float recoilYawMin;
	uint64_t RecoilYawMax = 0x1C; //  RecoilProperties-> public float recoilYawMax;
	uint64_t RecoilPitchMin = 0x20; // RecoilProperties-> public float recoilPitchMin;
	uint64_t RecoilPitchMax = 0x24; // RecoilProperties-> public float recoilPitchMax;
	uint64_t ProjectileThickness = 0x34; // Projectile-> public float thickness;
public:
	BaseProjectile(uint64_t address);
	bool IsValidWeapon();
	void WriteRecoilYaw(VMMDLL_SCATTER_HANDLE handle, uint32_t itemid, int percent);
	void WriteRecoilPitch(VMMDLL_SCATTER_HANDLE handle, uint32_t itemid, int percent);
};
