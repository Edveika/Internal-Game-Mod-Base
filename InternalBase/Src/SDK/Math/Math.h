#pragma once
#include <cmath>

#define PI 3.1415927f

struct Vector4 { float x, y, z, w; };

struct Vector3
{
	float x, y, z;

	Vector3() {};
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float length() const { return sqrtf(x * x + y * y + z * z); }
	Vector3 normalize() const { return *this * (1 / length()); }
	float distance(const Vector3& rhs) const { return (*this - rhs).length(); }
};

struct Vector2
{
	float x, y;

	Vector2() {};
	Vector2(const float x, const float y) : x(x), y(y) {}
	Vector2 operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2 operator * (const float& rhs) const { return Vector2(x * rhs, y * rhs); }
	Vector2 operator / (const float& rhs) const { return Vector2(x / rhs, y / rhs); }
	Vector2& operator += (const Vector2& rhs) { return *this = *this + rhs; }
	Vector2& operator -= (const Vector2& rhs) { return *this = *this - rhs; }
	Vector2& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector2& operator /= (const float& rhs) { return *this = *this / rhs; }
	float length() const { return sqrtf(x * x + y * y); }
	Vector2 normalize() const { return *this * (1 / length()); }
	float distance(const Vector2& rhs) const { return (*this - rhs).length(); }
};

namespace Math
{
	float radian_to_degree(float radian);
	float degree_to_radian(float degree);
	Vector3 radian_to_degree(Vector3 radians);
	Vector3 degree_to_radian(Vector3 degrees);
	bool world_to_screen(Vector3 Position, Vector2& ScreenPosition, float viewMatrix[16], int screenWidth, int screenHeight);
	Vector3 subtract(Vector3 src, Vector3 dst);
	float pythagorean_theorem(Vector3 vec);
	float distance(Vector3 src, Vector3 dst);
	Vector3 calc_angle(Vector3 src, Vector3 dst);
}