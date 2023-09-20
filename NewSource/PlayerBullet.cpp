#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velosity) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/PBullet/PBullet.png");
	worldTransform_.Initialize();
	velosity_ = velosity;

	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::atan2f(velosity_.x, velosity_.z);

	float velosityXZ = sqrtf((velosity_.x * velosity_.x) + (velosity_.z * velosity_.z));

	worldTransform_.rotation_.x = std::atan2f(-velosity_.y, velosityXZ);

}
void PlayerBullet::Update() {

	worldTransform_.translation_ += velosity_;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}
void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
void PlayerBullet::OnCollision() { isDead_ = true; }
Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPosition{};

	worldPosition.x = worldTransform_.translation_.x;
	worldPosition.y = worldTransform_.translation_.y;
	worldPosition.z = worldTransform_.translation_.z;

	return worldPosition;
}