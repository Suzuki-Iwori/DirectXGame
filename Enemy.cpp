#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velosity) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	worldTransform_.Initialize();
	velosity_ = velosity;

	ApproachingInitialize();

	worldTransform_.translation_ = position;
}
void Enemy::Update() {

	(this->*MoveFanction[static_cast<size_t>(phase_)])();

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();

}
void Enemy::ApproachingMove() {
	worldTransform_.translation_ += velosity_;
	fireCount--;

	if (fireCount <= 0) {
		Fire();

		fireCount = kFireInterval;

	}

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}
void Enemy::LeavingMove() { 
	worldTransform_.translation_ += {1.0f, 0.0f, 1.0f}; 
}

void Enemy::ApproachingInitialize() {

	fireCount = kFireInterval;

}

void Enemy::Fire() {
		const float bulletSpeed = -1.0f;
		Vector3 velosity(0.0f, 0.0f, bulletSpeed);

		velosity = TransformNormal(velosity, worldTransform_.matWorld_);

		EnemyBullet* newBullet = new EnemyBullet;
		newBullet->Initialize(model_, worldTransform_.translation_, velosity);

		enemyBullets_.push_back(newBullet);
}

void (Enemy::*Enemy::MoveFanction[])() = {
	&Enemy::ApproachingMove, 
	&Enemy::LeavingMove
};

void Enemy::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection);
	}

}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}
