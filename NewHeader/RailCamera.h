#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"

class RailCamera {
public:
	void Initialize(const Vector3& position, const Vector3& rotate);

	void Update();

	WorldTransform& GetWorldTransform() { return worldTransform_; };
	ViewProjection& GetViewProjection() { return viewProjection_; };

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};