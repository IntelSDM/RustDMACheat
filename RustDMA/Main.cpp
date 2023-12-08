#include "pch.h"
#include "Globals.h"
#include "DMAHandler.h"
#include "OcclusionCulling.h"
#include "MainCamera.h"
#include "ConvarGraphics.h"
#include "ConvarAdmin.h"
#include "ConsoleSystem.h"
#include "LocalPlayer.h"
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
	std::printf("PID: %X\n", TargetProcess.GetPID());
	TargetProcess.FixDTB();
	std::printf("Base Address: 0x%X\n", TargetProcess.GetBaseAddress());
	std::printf("Game Assembly: 0x%X\n", TargetProcess.GetModuleAddress(L"GameAssembly.dll"));
	std::printf("Unity Player: 0x%X\n", TargetProcess.GetModuleAddress(L"UnityPlayer.dll"));

	OcclusionCulling* occlusionculling = new OcclusionCulling();
	occlusionculling->WriteDebugSettings(DebugFilter::Dynamic);
	occlusionculling->WriteLayerMask(131072);
	MainCamera* maincamera = new MainCamera();
	ConvarGraphics* convargraphics = new ConvarGraphics();
	convargraphics->WriteFOV(100);
	ConvarAdmin *convaradmin = new ConvarAdmin();
	convaradmin->ClearVisionInWater(true);
	convaradmin->SetAdminTime(12);
	ConsoleSystem* consolesystem = new ConsoleSystem();
	LocalPlayer* localplayer = new LocalPlayer();
	while (true) {}
}