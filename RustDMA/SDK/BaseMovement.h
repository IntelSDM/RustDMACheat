#pragma once
class BaseMovement
{
	uint64_t Class = 0x0;
	uint64_t GroundAngle = 0xCC; // private float groundAngle;
	uint64_t GroundAngleNew = 0xD0; //private float groundAngleNew;
	uint64_t MaxAngleClimbing = 0x98; //public float maxAngleClimbing;
	uint64_t MaxAngleWalking = 0x94; //public float maxAngleWalking;
	uint64_t GroundTime = 0xD4; // 	private float groundTime;
	uint64_t JumpTime = 0xD8; //private float jumpTime;
	uint64_t LandTime = 0xDC; //private float landTime;
public:
	BaseMovement(uint64_t address);
	float GetGroundAngle();
	float GetGroundAngleNew();
	float GetMaxAngleClimbing();
	float GetMaxAngleWalking();
	float GetGroundTime();
	float GetJumpTime();
	float GetLandTime();
	// Create a scatter handle and then execute it after the function calls.
	void WriteGroundAngle(VMMDLL_SCATTER_HANDLE handle, float angle);
	void WriteGroundAngleNew(VMMDLL_SCATTER_HANDLE handle,float angle);
	void WriteMaxAngleClimbing(VMMDLL_SCATTER_HANDLE handle,float angle);
	void WriteMaxAngleWalking(VMMDLL_SCATTER_HANDLE handle,float angle);
	void WriteGroundTime(VMMDLL_SCATTER_HANDLE handle, float time);
	void WriteJumpTime(VMMDLL_SCATTER_HANDLE handle, float time);
	void WriteLandTime(VMMDLL_SCATTER_HANDLE handle, float time);
};