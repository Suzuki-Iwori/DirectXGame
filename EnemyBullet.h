#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <cassert>

class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velosity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	bool IsDead() const { return isDead_; };
	Vector3 GetWorldPosition();

private:
	const int32_t kLifeCount = 60 * 20;
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velosity_{};
	int32_t deathTimer_ = kLifeCount;
	bool isDead_ = false;
};
