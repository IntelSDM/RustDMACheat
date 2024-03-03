#pragma once

extern Vector2 MousePos;
extern std::map<std::string, HCURSOR> Cursors;
extern WPARAM Char;
LRESULT CALLBACK InputWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool IsKeyDown(int key);
bool IsKeyClicked(int key);
void SetInput();
bool IsMouseInRectangle(int x, int y, int width, int height);
bool IsMouseInRectangle(Vector2 pos, Vector2 Dimensions);
bool IsMouseInTriangle(Vector2 point1, Vector2 point2, Vector2 point3);

void SetCurrentCursor(std::string name);
void CreateCursor(std::string name, HCURSOR cursor);
HCURSOR GetCurrentCursor();
