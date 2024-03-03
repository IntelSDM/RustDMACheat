#include "pch.h"
#include "vector.h"

#include <cmath>
#include <emmintrin.h>

Vector2::Vector2() : x(0), y(0) { }

Vector2::Vector2(float _x, float _y) : x(_x), y(_y) { }

Vector2::~Vector2() { }

Vector2 Vector2::operator*(Vector2 a)
{
    return Vector2(x * a.x, y * a.y);
}

Vector2 Vector2::operator/(Vector2 a)
{
    return Vector2(x / a.x, y / a.y);
}

Vector2 Vector2::operator+(Vector2 a)
{
    return Vector2(x + a.x, y + a.y);
}

Vector2 Vector2::operator-(Vector2 a)
{
    return Vector2(x - a.x, y - a.y);
}

bool Vector2::operator==(Vector2 a)
{
    return x == a.x && y == a.y;
}

bool Vector2::operator!=(Vector2 a)
{
    return x != a.x || y != a.y;
}

bool Vector2::IsZero() const
{
    return !x && !y;
}

Vector2 Vector2::Zero()
{
    return Vector2();
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
    const auto difference = Vector2(
        a.x - b.x,
        a.y - b.y);

    return sqrt(
        powf(difference.x, 2.0f) +
        powf(difference.y, 2.0f));
}

Vector3::Vector3() : x(0), y(0), z(0) { }

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

Vector3::~Vector3() { }

Vector3 Vector3::operator*(Vector3 a)
{
    return Vector3(x * a.x, y * a.y, z * a.z);
}

Vector3 Vector3::operator*(float f)
{
    return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator/(Vector3 a)
{
    return Vector3(x / a.x, y / a.y, z / a.z);
}

Vector3 Vector3::operator/(float f)
{
    return Vector3(x / f, y / f, z / f);
}

Vector3 Vector3::operator+(Vector3 a)
{
    return Vector3(x + a.x, y + a.y, z + a.z);
}

Vector3 Vector3::operator-(Vector3 a)
{
    return Vector3(x - a.x, y - a.y, z - a.z);
}

bool Vector3::operator==(Vector3 a)
{
    return x == a.x && y == a.y && z == a.z;
}

bool Vector3::operator!=(Vector3 a)
{
    return x != a.x || y != a.y || z != a.z;
}

bool Vector3::IsZero() const
{
    return !x && !y && !z;
}

float Vector3::Dot(Vector3 left, Vector3 right)
{
    return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

float Vector3::Distance(Vector3 a, Vector3 b)
{
    const auto difference = Vector3(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z);

    return sqrt(
        powf(difference.x, 2.0f) +
        powf(difference.y, 2.0f) +
        powf(difference.z, 2.0f)
    );

}

int Vector3::FormattedDistance(Vector3 a, Vector3 b)
{
    const auto difference = Vector3(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z);

    int distance = sqrt(
        powf(difference.x, 2.0f) +
        powf(difference.y, 2.0f) +
        powf(difference.z, 2.0f)
    );
    return (int)(5 * round(distance / 5.0));
}

Vector3 Vector3::Zero()
{
    return Vector3();
}

float Vector3::Length() const
{
    return sqrt((x * x) + (y * y) + (z * z));
}

float Vector3::LengthSqr() const
{
    return (x * x) + (y * y) + (z * z);
}

Vector3 Vector3::Clamp() const
{
    Vector3 angles = *this;

    if (angles.x > 89.f)
        angles.x -= 360.f;
    else if (angles.x < -89.f)
        angles.x += 360.f;

    if (angles.y > 180.f)
        angles.y -= 360.f;
    else if (angles.y < -180.f)
        angles.y += 360.f;

    angles.z = 0.f;
    return angles;
}

Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t)
{
    return a * (1.f - t) + b * t;
}


Vector3 ViewMatrix::Transform(const Vector3 vector) const
{
    Vector3 transformed;

    transformed.x = vector.y * matrix[0][1] + vector.x * matrix[0][0] + vector.z * matrix[0][2] + matrix[0][3];
    transformed.y = vector.y * matrix[1][1] + vector.x * matrix[1][0] + vector.z * matrix[1][2] + matrix[1][3];
    transformed.z = vector.y * matrix[3][1] + vector.x * matrix[3][0] + vector.z * matrix[3][2] + matrix[3][3];

    return transformed;
}