#include "pch.h"
#include "Globals.h"
#include "DMAHandler.h"
#include "OcclusionCulling.h"
DMAHandler TargetProcess = DMAHandler(L"RustClient.exe");

void main()
{

	if (!TargetProcess.IsInitialized())
	{
		DebugBreak();
		std::printf("Unable To Connect To FPGA Device");
	}
	//is the PID valid?
	if (!TargetProcess.GetPID())
	{
		std::printf("Game Isn't Open");
		Sleep(1000);
		TargetProcess = DMAHandler(L"RustClient.exe");
	}
	std::printf("PID: 0x%X\n", TargetProcess.GetPID());
	TargetProcess.FixDTB();
	std::printf("Base Address: 0x%llX\n", TargetProcess.GetBaseAddress());
	std::printf("Game Assembly: 0x%X\n", TargetProcess.GetModuleAddress(L"GameAssembly.dll"));
	std::printf("Unity Player: 0x%X\n", TargetProcess.GetModuleAddress(L"UnityPlayer.dll"));
	// 0x33CEB00
	OcclusionCulling* occlusionculling = new OcclusionCulling();
	occlusionculling->WriteDebugSettings(DebugFilter::Dynamic);
	occlusionculling->WriteLayerMask(131072);
	//uint64_t convarculling = TargetProcess.Read<uint64_t>(TargetProcess.GetModuleAddress(L"GameAssembly.dll") + (ULONG64)0x3436190); // OcclusionCulling_c*
//	uint64_t staticfield = TargetProcess.Read<uint64_t>(convarculling + (ULONG64)0xB8); // static
//	uint64_t instance = TargetProcess.Read<uint64_t>(staticfield + (ULONG64)0x70); // instance 
//	uint64_t debug = TargetProcess.Read<uint64_t>(instance + (ULONG64)0x1A0);  // debugSettings
//	std::printf("ConVarCulling: 0x%llX\n", convarculling);
//	std::printf("StaticField: 0x%llX\n", staticfield);
//	std::printf("Instance: 0x%llX\n", instance);

/*
std::printf("Debug: 0x%llX\n", debug);

	if (!TargetProcess.Write<int>(staticfield + 0x8C, 1))
	{
std::printf("Failed To Write");
	
	}
	if (!TargetProcess.Write<int>(debug + 0x20, 131072))
	{
		std::printf("Failed To Write");

	}
	*/
	while (true) {}
}