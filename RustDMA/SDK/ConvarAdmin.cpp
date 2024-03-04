#include "Pch.h"
#include "ConvarAdmin.h"
#include "Globals.h"

ConvarAdmin::ConvarAdmin()
{
	printf("[ConvarAdmin] Initialized\n");
	uint64_t graphics = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(LIT("GameAssembly.dll")) + Class); // Get Class Start Address
	printf("[ConvarAdmin] ConvarAdmin: 0x%llX\n", graphics);
	this->StaticField = TargetProcess.Read<uint64_t>(graphics + StaticField); // Set Static Padding
	printf("[ConvarAdmin] Static Fields: 0x%llX\n", StaticField);
}

void ConvarAdmin::ClearVisionInWater(bool value)
{
	if (!TargetProcess.Write<bool>(StaticField + AdminUnderWaterEffect, !value))
		printf("[ConvarAdmin] Failed to write AdminUnderWaterEffect\n");
}
void ConvarAdmin::SetAdminTime(float value)
{
	if (value == 0)
		value = -1;// this is just easier to manage with a slider
	if (!TargetProcess.Write<float>(StaticField + AdminTime, value))
		printf("[ConvarAdmin] Failed to write AdminTime\n");
}
