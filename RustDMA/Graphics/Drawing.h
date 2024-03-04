#pragma once
void DrawText(int x, int y, std::wstring text, std::string font, int fontsize, MyColour colour, FontAlignment alignment);
void OutlineCircle(int x, int y, float radius, float linewidth, MyColour colour);
void FilledCircle(int x, int y, float radius, MyColour colour);
void OutlineRectangle(int x, int y, int width, int height, int linewidth, MyColour colour);
void FilledRectangle(int x, int y, int width, int height, MyColour colour);
void OutlineRoundedRectangle(int x, int y, int width, int height, int linewidth, int rounding, MyColour colour);
void FilledRoundedRectangle(int x, int y, int width, int height, int rounding, MyColour colour);
void FilledLine(int xstart, int ystart, int xend, int yend, int width, MyColour colour);
void FilledLineAliased(int xstart, int ystart, int xend, int yend, int width, MyColour colour);
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y, int width, int height);
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y);
void SaturationSlider(int x, int y, int width, int height, MyColour colour);
void AlphaSlider(int x, int y, int width, int height, MyColour col);
void HueSlider(int x, int y, int width, int height);
void FilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, MyColour colour);
