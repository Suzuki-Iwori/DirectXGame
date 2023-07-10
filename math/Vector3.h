#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

Vector3& operator+=(Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3& operator-=(Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3& operator*=(Vector3& v, const float& f);
Vector3 operator*(const Vector3& v, const float& f);
float Dot(const Vector3& v1, const Vector3& v2);
Vector3 Normalize(const Vector3& vector);
Vector3 Slarp(Vector3& velocity, const Vector3& toVelocity, const float& t);