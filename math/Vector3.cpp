#include "Vector3.h"

Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	Vector3 result {};

	result.x += v1.x + v2.x;
	result.y += v1.y + v2.y;
	result.z += v1.z + v2.z;

	v1 = result;

	return v1;

}