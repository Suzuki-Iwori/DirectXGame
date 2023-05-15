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

	switch (phase_) { 
	case Phase::Approach:
	default:
		worldTransform_.translation_ += velosity_;
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		worldTransform_.translation_ += {0.0f, 1.0f, 0.0f};
		break;

	}

	worldTransform_.UpdateMatrix();

}
void Enemy::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}