#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velosity) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	worldTransform_.Initialize();
	velosity_ = velosity;

	worldTransform_.translation_ = position;
}
void Enemy::Update() {

	worldTransform_.translation_ += velosity_;

	worldTransform_.UpdateMatrix();

}
void Enemy::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}