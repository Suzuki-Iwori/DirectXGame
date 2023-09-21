#include "EnemyBullet.h"
#include "GameScene.h"

void EnemyBullet::Initialize(
    Model* model, const Vector3& position, const Vector3& velosity) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("./Resources/enemyBullet/enemyBullet.png");
	worldTransform_.Initialize();
	velosity_ = velosity;

	worldTransform_.translation_ = position;

	worldTransform_.scale_.x = 1.0f;
	worldTransform_.scale_.y = 1.0f;
	worldTransform_.scale_.z = 1.0f;

	worldTransform_.rotation_.y = std::atan2f(velosity_.x, velosity_.z);

	float velosityXZ = sqrtf((velosity_.x * velosity_.x) + (velosity_.z * velosity_.z));

	worldTransform_.rotation_.x = std::atan2f(-velosity_.y, velosityXZ);

}
void EnemyBullet::Update() {

	worldTransform_.translation_ += velosity_;

	worldTransform_.rotation_.z += 0.1f;
	worldTransform_.rotation_.y += 0.1f;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}
void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
void EnemyBullet::OnCollision() { isDestroyed_ = true; }
Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPosition{};

	worldPosition.x = worldTransform_.translation_.x;
	worldPosition.y = worldTransform_.translation_.y;
	worldPosition.z = worldTransform_.translation_.z;

	return worldPosition;
}