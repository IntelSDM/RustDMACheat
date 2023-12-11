#include "pch.h"
#include "Globals.h"
#include "DMAHandler.h"
#include "OcclusionCulling.h"
#include "MainCamera.h"
#include "ConvarGraphics.h"
#include "ConvarAdmin.h"
#include "ConsoleSystem.h"
#include "LocalPlayer.h"
#include "BaseNetworkable.h"
#include "BasePlayer.h"
#include "TODSky.h"
#include "BaseProjectile.h"
DMAHandler TargetProcess = DMAHandler(L"RustClient.exe");
BasePlayer* BaseLocalPlayer = nullptr;
void MainThread();
// each time we reinitialize localplayer
void PerServerVariables()
{
	LocalPlayer* localplayer = new LocalPlayer();
	BasePlayer* baseplayer = new BasePlayer(localplayer->GetBasePlayer());
	BaseLocalPlayer = new BasePlayer(localplayer->GetBasePlayer());
}
void SetupCvars()
{
	OcclusionCulling* occlusionculling = new OcclusionCulling();
	occlusionculling->WriteDebugSettings(DebugFilter::Dynamic);
	occlusionculling->WriteLayerMask(131072);
	MainCamera* maincamera = new MainCamera();
	ConvarGraphics* convargraphics = new ConvarGraphics();
	convargraphics->WriteFOV(100);
	ConvarAdmin* convaradmin = new ConvarAdmin();
	convaradmin->ClearVisionInWater(true);
	convaradmin->SetAdminTime(12);
	ConsoleSystem* consolesystem = new ConsoleSystem();
	TODSky* todsky = new TODSky();
}
void Intialize()
{
	PerServerVariables();
	while (!BaseLocalPlayer->IsPlayerValid())
	{
		Sleep(4000);

		Intialize(); // wait till localplayer is valid.
	}
	SetupCvars();
	MainThread();
}
void MainThread()
{
	BaseProjectile* currentweapon = nullptr;
	while (true)
	{

		auto handle = TargetProcess.CreateScatterHandle();
		// spiderman
		BaseLocalPlayer->GetBaseMovement()->WriteGroundAngle(handle, 0.0f);
		BaseLocalPlayer->GetBaseMovement()->WriteGroundAngleNew(handle, 0.0f);
		BaseLocalPlayer->GetBaseMovement()->WriteMaxAngleClimbing(handle, 999.0f);
		BaseLocalPlayer->GetBaseMovement()->WriteMaxAngleWalking(handle, 999.0f);
		BaseLocalPlayer->WritePlayerFlag(handle, PlayerFlags::IsAdmin); 
		// held weapon
		BaseLocalPlayer->UpdateActiveItemID(handle);

		TargetProcess.ExecuteScatterWrite(handle);
		TargetProcess.CloseScatterHandle(handle);
		BaseLocalPlayer->SetupBeltContainerList();
		Item* helditem = BaseLocalPlayer->GetActiveItem();
		if(helditem != nullptr)
			currentweapon = helditem->GetBaseProjectile();
		if (currentweapon != nullptr && helditem != nullptr)
		{
			if (currentweapon->IsValidWeapon())
			{
				uint32_t itemid = helditem->GetItemID();
				if (itemid != 0 && helditem != nullptr)
				{

					currentweapon->WriteRecoilPitch(itemid, 0);
					currentweapon->WriteRecoilYaw(itemid, 0);
				}
			}
		}
		/*
		This needs to be done in a fast af loop, Heavily intensive
		// bright night, honestly useless.
	//	todsky->WriteNightLightIntensity(25.0f);
	//	todsky->WriteNightAmbientMultiplier(4.0f);
		// Bright Caves.
//		todsky->WriteDayAmbientMultiplier(2.0f);
*/

		Sleep(100);
	}
}
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

	Intialize();
	
	
	
	//BaseNetworkable* basenetworkable = new BaseNetworkable();
	//basenetworkable->ItterateEntities();
}