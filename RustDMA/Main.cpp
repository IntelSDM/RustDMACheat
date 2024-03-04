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
	auto handle = TargetProcess.CreateScatterHandle();
	BaseLocalPlayer = std::make_shared <BasePlayer>(localplayer->GetBasePlayer(),handle);
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	BaseLocalPlayer->InitializePlayerList();
}
void SetupCvars()
{
	/*std::shared_ptr < OcclusionCulling> occlusionculling = std::make_shared <OcclusionCulling>();
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
		BaseLocalPlayer->WritePlayerFlag(PlayerFlags::IsAdmin);*/
}
std::shared_ptr<CheatFunction> CachePlayers = std::make_shared<CheatFunction>(2000, []() {
		BaseLocalPlayer->CachePlayers();
	});
void Caching()
{
	CachePlayers->Execute();

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
	CachePlayers->Execute();
	
}
void main()
{
	if (!TargetProcess.Init("RustClient.exe"))
	{
		printf("Failed to initialize process\n");
		return;
	}
	TargetProcess.GetBaseAddress("GameAssembly.dll");
	Intialize();
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
		Caching();
	}
	CleanD2D();
	return msg.wParam;
}
