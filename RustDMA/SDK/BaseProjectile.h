#pragma once
class BaseProjectile
{
	uint64_t Class = 0x0;
	uint64_t RecoilProperties = 0x2E0; //	public RecoilProperties recoil;

	// Part of  RecoilProperties
	uint64_t RecoilOverride = 0x80; //	public RecoilProperties newRecoilOverride;
	uint64_t RecoilYawMin = 0x18; // RecoilProperties-> public float yawMin;
	uint64_t RecoilYawMax = 0x1C; //  RecoilProperties->public float yawMax;
	uint64_t RecoilPitchMin = 0x20; // RecoilProperties-> public float pitchMin;
	uint64_t RecoilPitchMax = 0x24; // RecoilProperties-> public float pitchMax;
	uint64_t ProjectileThickness = 0x2c; // Projectile-> public float thickness;
public:
	BaseProjectile(uint64_t address);
	bool IsValidWeapon();
	void WriteRecoilYaw(VMMDLL_SCATTER_HANDLE handle, uint32_t itemid, int percent);
	void WriteRecoilPitch(VMMDLL_SCATTER_HANDLE handle, uint32_t itemid, int percent);
};