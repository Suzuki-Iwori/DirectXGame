#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <cassert>

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};