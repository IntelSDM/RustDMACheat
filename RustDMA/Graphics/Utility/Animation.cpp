#include "pch.h"
#include "Animation.h"

float InOutSine(float t)
{
	return -0.5f * (std::cos(3.14f * t) - 1.0f);
}

MyColour InterpolateColour(const MyColour& colour1, const MyColour& colour2, float progress)
{
	float r = colour1.r + (colour2.r - colour1.r) * progress;
	float g = colour1.g + (colour2.g - colour1.g) * progress;
	float b = colour1.b + (colour2.b - colour1.b) * progress;

	return Colour(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(255));
}