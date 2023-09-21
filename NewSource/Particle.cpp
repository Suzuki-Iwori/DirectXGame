#include "Particle.h"
#include "Random.h"

void Particle::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_.x = 1.0f;
	worldTransform_.scale_.y = 1.0f;
	worldTransform_.scale_.z = 1.0f;

	velosity_.x = (float)(SetRandom(-100, 100) * 0.01);
	velosity_.y = (float)(SetRandom(-100, 100) * 0.01);
	velosity_.z = (float)(SetRandom(-100, 100) * 0.01);

	velosity_ = Normalize(velosity_);

}

void Particle::Update() { 
	worldTransform_.translation_ += velosity_;

	worldTransform_.scale_.x -= 0.05f;
	worldTransform_.scale_.y -= 0.05f;
	worldTransform_.scale_.z -= 0.05f;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void Particle::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}