#include "Pch.h"
#include "BaseMovement.h"
#include "Globals.h"

BaseMovement::BaseMovement(uint64_t address)
{
	this->Class = address;
}

float BaseMovement::GetGroundAngle()
{
	return TargetProcess.Read<float>(Class + GroundAngle);
}

float BaseMovement::GetGroundAngleNew()
{
	return TargetProcess.Read<float>(Class + GroundAngleNew);
}

float BaseMovement::GetMaxAngleClimbing()
{
	return TargetProcess.Read<float>(Class + MaxAngleClimbing);
}

float BaseMovement::GetMaxAngleWalking()
{
	return TargetProcess.Read<float>(Class + MaxAngleWalking);
}

float BaseMovement::GetGroundTime()
{
	return TargetProcess.Read<float>(Class + GroundTime);
}

float BaseMovement::GetJumpTime()
{
	return TargetProcess.Read<float>(Class + JumpTime);
}

float BaseMovement::GetLandTime()
{
	return TargetProcess.Read<float>(Class + LandTime);
}

void BaseMovement::WriteGroundAngle(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	TargetProcess.AddScatterWriteRequest<float>(handle, Class + GroundAngle, angle);
}

void BaseMovement::WriteGroundAngleNew(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	TargetProcess.AddScatterWriteRequest<float>(handle, Class + GroundAngleNew, angle);
}

void BaseMovement::WriteMaxAngleClimbing(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	TargetProcess.AddScatterWriteRequest<float>(handle, Class + MaxAngleClimbing, angle);
}

void BaseMovement::WriteMaxAngleWalking(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	TargetProcess.AddScatterWriteRequest<float>(handle, Class + MaxAngleWalking, angle);
	
}

void BaseMovement::WriteGroundTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	TargetProcess.AddScatterWriteRequest<float>(handle, Class + GroundTime, time);
}

void BaseMovement::WriteJumpTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	TargetProcess.AddScatterWriteRequest<float>(handle, Class + JumpTime, time);
}

void BaseMovement::WriteLandTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	TargetProcess.AddScatterWriteRequest<float>(handle, Class + LandTime, time);
}