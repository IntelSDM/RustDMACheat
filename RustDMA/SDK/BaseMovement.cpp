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
	if(!TargetProcess.QueueScatterWriteEx<float>(handle, Class + GroundAngle, angle))
		printf("[BaseMovement] Failed to write GroundAngle\n");
}

void BaseMovement::WriteGroundAngleNew(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	if(!TargetProcess.QueueScatterWriteEx<float>(handle, Class + GroundAngleNew, angle))
		printf("[BaseMovement] Failed to write GroundAngleNew\n");
}

void BaseMovement::WriteMaxAngleClimbing(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	if(!TargetProcess.QueueScatterWriteEx<float>(handle, Class + MaxAngleClimbing, angle))
		printf("[BaseMovement] Failed to write MaxAngleClimbing\n");
}

void BaseMovement::WriteMaxAngleWalking(VMMDLL_SCATTER_HANDLE handle, float angle)
{
	if(!TargetProcess.QueueScatterWriteEx<float>(handle, Class + MaxAngleWalking, angle))
		printf("[BaseMovement] Failed to write MaxAngleWalking\n");
}

void BaseMovement::WriteGroundTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	if(!TargetProcess.QueueScatterWriteEx<float>(handle, Class + GroundTime, time))
		printf("[BaseMovement] Failed to write GroundTime\n");
}

void BaseMovement::WriteJumpTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	if(!TargetProcess.QueueScatterWriteEx<float>(handle, Class + JumpTime, time))
		printf("[BaseMovement] Failed to write JumpTime\n");
}

void BaseMovement::WriteLandTime(VMMDLL_SCATTER_HANDLE handle, float time)
{
	if(!TargetProcess.QueueScatterWriteEx<float>(handle, Class + LandTime, time))
		printf("[BaseMovement] Failed to write LandTime\n");
}