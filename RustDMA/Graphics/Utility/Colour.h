#pragma once
D2D1::ColorF Colour(UINT8 R, UINT8 G, UINT8 B, UINT8 A = 255);

//Fuck you Microsoft for not having a default constructor that will work for mappy map :(
//should by default work even if you just passthrough D2D1 Color to it (:
struct MyColour
{
	float r, g, b, a;
	MyColour() = default;

	MyColour(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a)
	{
	}

	MyColour(int r, int g, int b, int a = 255) : r(static_cast<float>(r) / 255.0f), g(static_cast<float>(g) / 255.0f), b(static_cast<float>(b) / 255.0f), a(static_cast<float>(a) / 255.0f)
	{
	}

	MyColour(D2D1::ColorF col) : r(col.r), g(col.g), b(col.b), a(col.a)
	{
	}

	MyColour Modify(float r, float g, float b, float a)
	{
		return MyColour(r, g, b, a);
	}

	/*Convert to d2d*/
	D2D1::ColorF Get() { return D2D1::ColorF(r, g, b, a); }
};

struct HsvColour
{
	float H;
	float S;
	float V;
};

extern std::map<std::string, MyColour> MenuColours;

D2D1::ColorF HueToRGB(float hue);
float RGBToHue(float r, float g, float b);
D2D1::ColorF HsvToRgb(float hue, float saturation, float value, float alpha);
HsvColour RgbToHsv(float r, float g, float b);
void SetColour(std::string name, D2D1::ColorF col);
