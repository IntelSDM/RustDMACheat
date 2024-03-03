#include "pch.h"
#include "Globals.h"
#include "memory.h"
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
#include "Init.h"
#include "GUI.h"
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
	if (!TargetProcess.Init("RustClient.exe"))
	{
		printf("Failed to initialize process\n");
		return;
	}
	TargetProcess.GetBaseAddress("GameAssembly.dll");
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InputWndProc(hWnd, message, wParam, lParam);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, LIT("CONIN$"), LIT("r"), stdin);
	freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stderr);
	freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stdout);
	printf(LIT("Debugging Window:\n"));

	main();
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"GUI Framework";
	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, L"GUI Framework",
		WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return -1;


	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	ShowWindow(hWnd, nCmdShow);

	InitD2D(hWnd);
	CreateGUI();
	MSG msg;
	SetProcessDPIAware();
	SetInput();
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		RenderFrame();
	}
	CleanD2D();
	return msg.wParam;
}
