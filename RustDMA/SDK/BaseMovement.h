#pragma once
#include "DMAHandler.h"
class BaseMovement
{
	uint64_t Class = 0x0;
	uint64_t GroundAngle = 0xC4; // private float groundAngle;
	uint64_t GroundAngleNew = 0xC8; //private float groundAngleNew;
	uint64_t MaxAngleClimbing = 0x90; //public float maxAngleClimbing;
	uint64_t MaxAngleWalking = 0x8c; //public float maxAngleWalking;
public:
	BaseMovement(uint64_t address);
	float GetGroundAngle();
	float GetGroundAngleNew();
	float GetMaxAngleClimbing();
	float GetMaxAngleWalking();
	// Create a scatter handle and then execute it after the function calls.
	void WriteGroundAngle(VMMDLL_SCATTER_HANDLE handle, float angle);
	void WriteGroundAngleNew(VMMDLL_SCATTER_HANDLE handle,float angle);
	void WriteMaxAngleClimbing(VMMDLL_SCATTER_HANDLE handle,float angle);
	void WriteMaxAngleWalking(VMMDLL_SCATTER_HANDLE handle,float angle);
};