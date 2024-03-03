#pragma once
#include "Entity.h"

class Toggle : public Entity
{
protected:
	bool* Data;
	bool Tabbed = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
	Vector2 TextSize = Vector2(0, 0);
    MyColour RectangleColour;
    const float ClickedAnimationDuration = 0.09; // Duration of the size change animation in seconds
    float ClickedAnimationProgress = 0.0;
    const float FadeAnimationDuration = 1.8; // Duration of the color alpha change animation in seconds
    float FadeAnimationProgress = 0.0;
    float ClickedSize = 0;
    const float HoverAnimationDuration = 0.3; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;


public:
	Toggle(float x, float y, std::wstring text, bool* data);
	void Update();
	void Draw();
};
