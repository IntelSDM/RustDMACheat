#include "Pch.h"
#include "Globals.h"
#include "TODSky.h"

TODSky::TODSky()
{
	printf("[TODSky] Initialized\n");
	Class = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(LIT("GameAssembly.dll")) + Class);
	printf("[TODSky] Class: 0x%llX\n", Class);
	StaticField = TargetProcess.Read<uint64_t>(Class + StaticField); // static field and 0x0 is 	private static List<TOD_Sky> instances;
	StaticField = TargetProcess.Read<uint64_t>(StaticField + 0x0);
	uint64_t intancevalues = TargetProcess.Read<uint64_t>(StaticField + 0x10); // 0x10 = list values
	Instance = TargetProcess.Read<uint64_t>(intancevalues + 0x20); // first value in instances list
	printf("[TODSky] Instance: 0x%llX\n", Instance);
	NightParameters = TargetProcess.Read<uint64_t>(Instance + NightParameters);
	DayParameters = TargetProcess.Read<uint64_t>(Instance + DayParameters);

}
void TODSky::WriteNightLightIntensity(VMMDLL_SCATTER_HANDLE handle, float value)
{
	
	if(!TargetProcess.AddScatterWriteRequest<float>(handle,NightParameters + LightIntensityNight, value))
			printf("[TODSky] Failed to write Night Light Intensity\n");
}
void TODSky::WriteNightAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle, float value)
{
	if(!TargetProcess.AddScatterWriteRequest<float>(handle,NightParameters + AmbientMultiplierNight, value))
				printf("[TODSky] Failed to write Night Ambient Multiplier\n");
}
void TODSky::WriteDayAmbientMultiplier(VMMDLL_SCATTER_HANDLE handle, float value)
{
	if(!TargetProcess.AddScatterWriteRequest<float>(handle,DayParameters + AmbientMultiplierDay, value))
					printf("[TODSky] Failed to write Day Ambient Multiplier\n");
}