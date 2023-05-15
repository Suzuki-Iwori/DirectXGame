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

	(this->*MoveFanction[static_cast<size_t>(phase_)])();

	worldTransform_.UpdateMatrix();

}
void Enemy::ApproachingMove() {
	worldTransform_.translation_ += velosity_;

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}
void Enemy::LeavingMove() { 
	worldTransform_.translation_ += {0.0f, 1.0f, 0.0f}; 
}

void (Enemy::*Enemy::MoveFanction[])() = {
	&Enemy::ApproachingMove, 
	&Enemy::LeavingMove
};

void Enemy::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}