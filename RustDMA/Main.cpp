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
#include "CheatFunction.h"
DMAHandler TargetProcess = DMAHandler(L"RustClient.exe");
std::shared_ptr<BasePlayer> BaseLocalPlayer = nullptr;
void MainThread();
bool SpiderMan = true;
bool NoRecoil = true;
int RecoilReduction = 25;
bool AdminFlag = true;
bool ChangeFov = false;
int Fov = 100;
bool ChangeTime = false;
int Time = 12;
bool BrightNight = true;
bool BrightCaves = false;
bool AdminEsp = false;
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
	if (AdminFlag)
		BaseLocalPlayer->WritePlayerFlag(PlayerFlags::IsAdmin);
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
std::shared_ptr <TODSky> TODSkyInstance;
auto MainLoopScatter = TargetProcess.CreateScatterHandle();
int ScatterCount = 0;
std::shared_ptr<CheatFunction> SkyMods = std::make_shared<CheatFunction>(7, []() {
	
	//This needs to be done in a fast af loop, Heavily intensive
	if (BrightNight)
	{
		ScatterCount++;
		TODSkyInstance->WriteNightLightIntensity(MainLoopScatter,25.0f);
		TODSkyInstance->WriteNightAmbientMultiplier(MainLoopScatter,4.0f);
	}
	if (BrightCaves)
	{
		ScatterCount++;
		TODSkyInstance->WriteDayAmbientMultiplier(MainLoopScatter,2.0f);
	}

	});


std::shared_ptr<CheatFunction> MovementMods = std::make_shared<CheatFunction>(50, []() {

	if (SpiderMan)
	{
		ScatterCount++;
		BaseLocalPlayer->GetBaseMovement()->WriteGroundAngle(MainLoopScatter, 0.0f);
		BaseLocalPlayer->GetBaseMovement()->WriteGroundAngleNew(MainLoopScatter, 0.0f);
		BaseLocalPlayer->GetBaseMovement()->WriteMaxAngleClimbing(MainLoopScatter, 999.0f);
		BaseLocalPlayer->GetBaseMovement()->WriteMaxAngleWalking(MainLoopScatter, 999.0f);
	}
	});
std::shared_ptr<CheatFunction> HotbarUpdater = std::make_shared<CheatFunction>(100, []() {

	if (NoRecoil)
		BaseLocalPlayer->UpdateActiveItemID(MainLoopScatter);// held weapon
	});
std::shared_ptr <BaseProjectile> CurrentWeapon = nullptr;
std::shared_ptr<CheatFunction> WeaponMods = std::make_shared<CheatFunction>(100, []() {
	if (NoRecoil)
	{
		BaseLocalPlayer->SetupBeltContainerList(); // this needs to be called to know the active item

		std::shared_ptr <Item> helditem = BaseLocalPlayer->GetActiveItem();
		if (helditem != nullptr)
			CurrentWeapon = helditem->GetBaseProjectile();
		if (CurrentWeapon != nullptr && helditem != nullptr)
		{
			if (CurrentWeapon->IsValidWeapon())
			{
				uint32_t itemid = helditem->GetItemID();
				if (itemid != 0 && helditem != nullptr)
				{

					CurrentWeapon->WriteRecoilPitch(itemid, RecoilReduction);
					CurrentWeapon->WriteRecoilYaw(itemid, RecoilReduction);
				}
			}
		}
	}
	});
void MainThread()
{
	TODSkyInstance = std::make_shared<TODSky>();
	
	while (true)
	{
	
		MainLoopScatter = TargetProcess.CreateScatterHandle();
		// spiderman
		MovementMods->Execute();
		HotbarUpdater->Execute();
		SkyMods->Execute();
		if(ScatterCount > 0)
		TargetProcess.ExecuteScatterWrite(MainLoopScatter);
		TargetProcess.CloseScatterHandle(MainLoopScatter);
		ScatterCount = 0;
		WeaponMods->Execute();
		
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