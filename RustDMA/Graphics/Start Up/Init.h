#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "windowscodecs.lib")

void CleanD2D();
void InitD2D(HWND hWnd);
void RenderFrame();
