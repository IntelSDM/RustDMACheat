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
std::shared_ptr<BasePlayer> BaseLocalPlayer = nullptr;
void MainThread();
bool SpiderMan = false;
bool NoRecoil = true;
int RecoilReduction = 25;
bool AdminFlag = true;
bool ChangeFov = true;
int Fov = 100;
bool ChangeTime = true;
int Time = 12;
bool BrightNight = false;
bool BrightCaves = false;
bool AdminEsp = true;
// each time we reinitialize localplayer
void PerServerVariables()
{
	std::shared_ptr <LocalPlayer> localplayer = std::make_shared <LocalPlayer>();
	std::shared_ptr <BasePlayer> baseplayer = std::make_shared <BasePlayer>(localplayer->GetBasePlayer());
	BaseLocalPlayer = std::make_shared <BasePlayer>(localplayer->GetBasePlayer());
}
void SetupCvars()
{
	std::shared_ptr < OcclusionCulling> occlusionculling = std::make_shared <OcclusionCulling>();
	if (AdminEsp)
	{
		occlusionculling->WriteDebugSettings(DebugFilter::Dynamic);
		occlusionculling->WriteLayerMask(131072);
	}
	std::shared_ptr <MainCamera> maincamera = std::make_shared < MainCamera>();
	std::shared_ptr <ConvarGraphics> convargraphics = std::make_shared <ConvarGraphics>();
	if(ChangeFov)
	convargraphics->WriteFOV(Fov);
	std::shared_ptr <ConvarAdmin> convaradmin = std::make_shared <ConvarAdmin>();
	convaradmin->ClearVisionInWater(true);
	if(ChangeTime)
	convaradmin->SetAdminTime(Time);
	std::shared_ptr <ConsoleSystem> consolesystem = std::make_shared <ConsoleSystem>();
	
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
	std::shared_ptr <TODSky> todsky = std::make_shared<TODSky>();
	std::shared_ptr <BaseProjectile> currentweapon = nullptr;
	while (true)
	{

		auto handle = TargetProcess.CreateScatterHandle();
		// spiderman
		if (SpiderMan)
		{
			BaseLocalPlayer->GetBaseMovement()->WriteGroundAngle(handle, 0.0f);
			BaseLocalPlayer->GetBaseMovement()->WriteGroundAngleNew(handle, 0.0f);
			BaseLocalPlayer->GetBaseMovement()->WriteMaxAngleClimbing(handle, 999.0f);
			BaseLocalPlayer->GetBaseMovement()->WriteMaxAngleWalking(handle, 999.0f);
		}
		if(AdminFlag)
		BaseLocalPlayer->WritePlayerFlag(handle, PlayerFlags::IsAdmin); 
		if(NoRecoil)
		BaseLocalPlayer->UpdateActiveItemID(handle);// held weapon

		TargetProcess.ExecuteScatterWrite(handle);
		TargetProcess.CloseScatterHandle(handle);
		if (NoRecoil)
		{
			BaseLocalPlayer->SetupBeltContainerList(); // this needs to be called to know the active item

			std::shared_ptr <Item> helditem = BaseLocalPlayer->GetActiveItem();
			if (helditem != nullptr)
				currentweapon = helditem->GetBaseProjectile();
			if (currentweapon != nullptr && helditem != nullptr)
			{
				if (currentweapon->IsValidWeapon())
				{
					uint32_t itemid = helditem->GetItemID();
					if (itemid != 0 && helditem != nullptr)
					{

						currentweapon->WriteRecoilPitch(itemid, RecoilReduction);
						currentweapon->WriteRecoilYaw(itemid, RecoilReduction);
					}
				}
			}
		}
		
		//This needs to be done in a fast af loop, Heavily intensive
		if (BrightNight)
		{
			todsky->WriteNightLightIntensity(25.0f);
			todsky->WriteNightAmbientMultiplier(4.0f);
		}
		if (BrightCaves)
		{
			todsky->WriteDayAmbientMultiplier(2.0f);
		}


		Sleep(10);
	}
}
void main()
{
	AllocConsole();
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