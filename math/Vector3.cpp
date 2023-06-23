#include "Vector3.h"
#include <cmath>

Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	Vector3 result {};

	result.x += v1.x + v2.x;
	result.y += v1.y + v2.y;
	result.z += v1.z + v2.z;

	v1 = result;

	return v1;

}
Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 result = v1;

	return result += v2;
}
Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x += v1.x - v2.x;
	result.y += v1.y - v2.y;
	result.z += v1.z - v2.z;

	v1 = result;

	return v1;
}
Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 result = v1;

	return result -= v2;
}
Vector3& operator*=(Vector3& v, const float& f) {
	Vector3 result{};

	result.x += v.x * f;
	result.y += v.y * f;
	result.z += v.z * f;

	v = result;

	return v;
}

Vector3 operator*(const Vector3& v, const float& f) {
	Vector3 result = v;

	return result *= f;
}
Vector3 Normalize(const Vector3& vector) {

	float length = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	return {(vector.x / length), (vector.y / length), (vector.z / length)};
}