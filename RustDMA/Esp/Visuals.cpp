#include "Pch.h"
#include "Globals.h"
#include "Visuals.h"
ViewMatrix CurrentMatrix;

Vector2 WorldToScreen(const Vector3& position)
{
	Vector3 transform(CurrentMatrix.matrix[0][3], CurrentMatrix.matrix[1][3], CurrentMatrix.matrix[2][3]);
	Vector3 right(CurrentMatrix.matrix[0][0], CurrentMatrix.matrix[1][0], CurrentMatrix.matrix[2][0]);
	Vector3 up(CurrentMatrix.matrix[0][1], CurrentMatrix.matrix[1][1], CurrentMatrix.matrix[2][1]);

	float w = Vector3::Dot(transform, position) + CurrentMatrix.matrix[3][3];

	if (w < 1.f)
		return Vector2(0, 0);
	float x = Vector3::Dot(right, position) + CurrentMatrix.matrix[3][0];
	float y = Vector3::Dot(up, position) + CurrentMatrix.matrix[3][1];

	return  Vector2((GetSystemMetrics(SM_CXSCREEN) / 2) * (1.f + x / w), (GetSystemMetrics(SM_CYSCREEN) / 2) * (1.f - y / w)); // grab screen.width and height later.


}

void RefreshViewMatrix()
{
	CurrentMatrix = Camera->GetViewMatrix();
}