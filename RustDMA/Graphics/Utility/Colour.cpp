#include "pch.h"
#include "Colour.h"

std::map<std::string, MyColour> MenuColours =
{
    /*Main Form colors*/
    {"Background", MyColour(20, 20, 20, 255)},
    {"Outline", MyColour(140, 140, 140, 255)},
    {"Header", MyColour(30, 30, 30, 255)},
    {"HeaderSeperator", MyColour(0, 150, 255, 255)},
    {"Text", MyColour(240, 240, 240, 255)},
    {"TextSelected", MyColour(0, 150, 255, 255)},
      {"TextHover", MyColour(0, 255, 0, 255)},

    {"Button", MyColour(80, 80, 80, 255)},
    {"ButtonHover", MyColour(100, 100, 100, 255)},
    {"ButtonActive", MyColour(120, 120, 120, 255)},
    {"ButtonOutline", MyColour(130, 130, 130, 255)},

    {"ComboBox", MyColour(80, 80, 80, 255)},
    {"ComboBoxHover", MyColour(150, 150, 150, 120)},
    {"ComboBoxOutline", MyColour(130, 130, 130, 255)},
    {"ComboBoxSelectedText", MyColour(0, 255, 150, 255)},
    {"ComboBoxDropDown", MyColour(150, 150, 150, 120)},
    {"ComboBoxSlider", MyColour(0, 255, 150, 255)},

    {"DropDown", MyColour(80, 80, 80, 255)},
    {"DropDownOutline", MyColour(130, 130, 130, 255)},
    {"DropDownSelectedText", MyColour(0, 255, 150, 255)},
    {"DropDownArrow", MyColour(0, 255, 150, 255)},
    {"DropDownActiveArrow", MyColour(150, 150, 150, 120)},
    {"DropDownSlider", MyColour(0, 255, 150, 255)},

    {"Keybind", MyColour(80, 80, 80, 255)},
    {"KeybindHover", MyColour(120, 120, 120, 255)},
    {"KeybindOutline", MyColour(130, 130, 130, 255)},

    {"Slider", MyColour(80, 80, 80, 255)},
    {"SliderInside", MyColour(0, 150, 255, 255)},
     {"SliderOutline", MyColour(20, 20, 20, 255)},
     {"SliderWidget", MyColour(255, 255, 255, 255)},

    {"Tab", MyColour(80, 80, 80, 255)},
    {"TabHover", MyColour(100, 100, 100, 255)},
    {"TabActive", MyColour(120, 120, 120, 255)},

    {"TextBox", MyColour(80, 80, 80, 255)},
    {"TextBoxOutline", MyColour(200, 200, 200, 255)},
    {"TextBoxHighlight", MyColour(0, 150, 255, 100)},
    {"TextBoxCurrent", MyColour(255, 255, 255, 255)}, // The current location of your cursor in the input box.
    {"TextBoxContextOutline", MyColour(120, 120, 120, 255)},
    {"TextBoxContextFirstLine", MyColour(255, 255, 255, 255)}, // First line that happens of the context menu

    {"Toggle", MyColour(80, 80, 80, 255)},
    {"ToggleOutline", MyColour(130, 130, 130, 255)},
    {"ToggleInside", MyColour(0, 150, 255, 255)},

    //{"CheckboxHover", MyColour(100, 100, 100, 255)},
    //{"CheckboxActive", MyColour(120, 120, 120, 255)},

};

D2D1::ColorF Colour(UINT8 R, UINT8 G, UINT8 B, UINT8 A)
{
    return D2D1::ColorF(static_cast<float>(R) / 255.0f, static_cast<float>(G) / 255.0f, static_cast<float>(B) / 255.0f, static_cast<float>(A) / 255.0f);
}


D2D1::ColorF HueToRGB(float hue)
{
    hue = fmod(hue, 360.0f);
    if (hue < 0)
        hue += 360.0f;

    int sector = static_cast<int>(hue / 60.0f);

    float fraction = (hue / 60.0f) - sector;

    float c = 1.0f;
    float x = 1.0f - std::abs(2 * fraction - 1);
    float m = 0.0f;
    D2D1::ColorF col = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f);

    switch (sector)
    {
    case 0:
        col.r = (c + m);
        col.g = (x + m);
        col.b = (m);
        break;
    case 1:
        col.r = (x + m);
        col.g = (c + m);
        col.b = (m);
        break;
    case 2:
        col.r = (m);
        col.g = (c + m);
        col.b = (x + m);
        break;
    case 3:
        col.r = (m);
        col.g = (x + m);
        col.b = (c + m);
        break;
    case 4:
        col.r = (x + m);
        col.g = (m);
        col.b = (c + m);
        break;
    case 5:
        col.r = (c + m);
        col.g = (m);
        col.b = (x + m);
        break;
    default:
        col.r = 0;
        col.g = 0;
        col.b = 0;
        break;
    }
    col.a = 1.0f;
    return col;
}

float RGBToHue(float r, float g, float b)
{
    float hue = 0.0f;
    float rnorm = r;
    float gnorm = g;
    float bnorm = b;

    float maxcolour = std::max({ rnorm, gnorm, bnorm });
    float mincolour = std::min({ rnorm, gnorm, bnorm });

    if (maxcolour == mincolour)
    {
        hue = 0.0f;
    }
    else
    {
        float delta = maxcolour - mincolour;
        if (maxcolour == rnorm)
            hue = 60.0f * fmod(((gnorm - bnorm) / delta), 6.0f);
        else if (maxcolour == gnorm)
            hue = 60.0f * (((bnorm - rnorm) / delta) + 2.0f);
        else if (maxcolour == bnorm)
            hue = 60.0f * (((rnorm - gnorm) / delta) + 4.0f);
    }

    if (hue < 0.0f)
        hue += 360.0f;

    return hue;
}

D2D1::ColorF HsvToRgb(float hue, float saturation, float value, float alpha)
{
    // Ensure hue is in the range [0, 360)
    hue = fmodf(hue, 360.0f);
    if (hue < 0.0f)
        hue += 360.0f;

    // Normalize saturation and value to [0, 1]
    saturation = std::clamp(saturation, 0.0f, 1.0f);
    value = std::clamp(value, 0.0f, 1.0f);

    // Algorithm to convert HSV to RGB
    float chroma = value * saturation;
    float hueprime = hue / 60.0f;
    float x = chroma * (1.0f - fabsf(fmodf(hueprime, 2.0f) - 1.0f));

    float r, g, b;
    if (0.0f <= hueprime && hueprime < 1.0f) {
        r = chroma;
        g = x;
        b = 0.0f;
    }
    else if (1.0f <= hueprime && hueprime < 2.0f) {
        r = x;
        g = chroma;
        b = 0.0f;
    }
    else if (2.0f <= hueprime && hueprime < 3.0f) {
        r = 0.0f;
        g = chroma;
        b = x;
    }
    else if (3.0f <= hueprime && hueprime < 4.0f) {
        r = 0.0f;
        g = x;
        b = chroma;
    }
    else if (4.0f <= hueprime && hueprime < 5.0f) {
        r = x;
        g = 0.0f;
        b = chroma;
    }
    else {
        r = chroma;
        g = 0.0f;
        b = x;
    }

    float m = value - chroma;
    r += m;
    g += m;
    b += m;
   
    return D2D1::ColorF(r, g, b, alpha);
}

HsvColour RgbToHsv(float r, float g, float b)
{
    float rnorm = r;
    float gnorm = g;
    float bnorm = b;
  //  printf("R: %f, G: %f, B: %f\n", rnorm, gnorm, bnorm);

    float maxcolor = std::max({ rnorm, gnorm, bnorm });
    float mincolor = std::min({ rnorm, gnorm, bnorm });

    float delta = maxcolor - mincolor;
    //printf("Delta: %f\n", delta);
    HsvColour hsv;

    // Value calculation
    hsv.V = maxcolor;
    if (maxcolor == 0 || delta == 0) {
        hsv.S = 0;
        hsv.H = 0; // undefined, set hue to 0
       
        return hsv;
    }
    else {
        hsv.S = delta / maxcolor;
    }


    // Hue calculation
    if (delta == 0)
        hsv.H = 0;
    else if (maxcolor == r)
        hsv.H = 60 * ((g - b) / delta);
    else if (maxcolor == g)
        hsv.H = 60 * ((b - r) / delta + 2);
    else
        hsv.H = 60 * ((r - g) / delta + 4);

    if (hsv.H < 0)
        hsv.H += 360;
    return hsv;
}

void SetColour(std::string name, D2D1::ColorF col)
{
    MenuColours[name] = MyColour(col);
}
