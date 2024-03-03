#pragma once
float InOutSine(float t);

template <typename T>
T Lerp(const T& start, const T& end, float t)
{
	if (t < 0.0f) t = 0.0f;
	else if (t > 1.0f) t = 1.0f;

	return start + t * (end - start);
}

MyColour InterpolateColour(const MyColour& colour1, const MyColour& colour2, float progress);