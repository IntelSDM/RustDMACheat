#include "Pch.h"
#include "OcclusionCulling.h"
#include "Globals.h"
OcclusionCulling::OcclusionCulling()
{
	printf("[OcclusionCulling] Initialized\n");
	uint64_t convarculling = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(LIT("GameAssembly.dll")) + Class); // Get Class Start Address
	printf("[OcclusionCulling] OcclusionCulling: 0x%llX\n", convarculling);
	this->StaticField = TargetProcess.Read<uint64_t>(convarculling + StaticField); // Set Static Padding
	printf("[OcclusionCulling] Static Fields: 0x%llX\n", StaticField);
	this->Instance = TargetProcess.Read<uint64_t>(StaticField +Instance); // instance 
	printf("[OcclusionCulling] Instance: 0x%llX\n", Instance);
	this->DebugSettings = TargetProcess.Read<uint64_t>(Instance + DebugSettings);  // debugSettings
	printf("[OcclusionCulling] DebugSettings: 0x%llX\n", Instance);
}

void OcclusionCulling::WriteLayerMask(int mask)
{
	
	if (!TargetProcess.Write<int>(this->DebugSettings + 0x20, mask))
	{
		std::printf("[OcclusionCulling] Mask Write Failure");

	}
}

void OcclusionCulling::WriteDebugSettings(DebugFilter debugfilter)
{
	if (!TargetProcess.Write<DebugFilter>(this->StaticField + 0x8C, debugfilter))
	{
		std::printf("[OcclusionCulling] Debug Settings Write Failure");

	}
}