#pragma once
#include <Vector3.h>

class Collider {
protected:
	float radius_ = 1.0f;

public:
	virtual void OnCollision() {}
	virtual Vector3 GetWorldPosition() = 0;

	void SetRadius(float setRadius) { radius_ = setRadius; }
	float GetRadius() { return radius_; }
};