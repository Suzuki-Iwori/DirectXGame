#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velosity) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/tex1.png");
	worldTransform_.Initialize();
	velosity_ = velosity;

	worldTransform_.translation_ = position;
}
void EnemyBullet::Update() {

	worldTransform_.translation_ += velosity_;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}
void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}