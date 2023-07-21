#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
#include "Collider.h"

class EnemyBullet : public Collider {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velosity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision() override;

	bool IsDead() const { return isDead_; };
	Vector3 GetWorldPosition() override;

private:
	const int32_t kLifeCount = 60 * 20;
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velosity_{};
	int32_t deathTimer_ = kLifeCount;
	bool isDead_ = false;
};
