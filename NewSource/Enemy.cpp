#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
#include "TimeCall.h"

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

	if (deathTimer_ < 0) {
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();

}
void Enemy::ApproachingMove() {
	worldTransform_.translation_ += velosity_;
	fireCount--;

	timeCalls_.remove_if([](TimeCall* timeCall) {
		if (timeCall->IsFinish()) {
			delete timeCall;
			return true;
		} else {
			return false;
		}
	});

	for (const auto& timeCall : timeCalls_) {
		timeCall->Update();
	}

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
		LeavingInitialize();
	}

}
void Enemy::LeavingMove() { 
	worldTransform_.translation_ += {1.0f, 0.0f, 1.0f}; 
	deathTimer_--;
}

void Enemy::ApproachingInitialize() {

	fireCount = kFireInterval;
	timeCalls_.push_back(new TimeCall(std::bind(&Enemy::FireReset, this), 60));

}

void Enemy::LeavingInitialize() { 
	timeCalls_.clear();
}

void Enemy::Fire() {
		const float bulletSpeed = 0.2f;

		Vector3 velosity =
	        Normalize((player_->GetWorldPosition() - GetWorldPosition()));

		velosity *= bulletSpeed;

		velosity = TransformNormal(velosity, worldTransform_.matWorld_);

		EnemyBullet* newBullet = new EnemyBullet;
		newBullet->Initialize(model_, worldTransform_.translation_, velosity);

		gameScene_->AddEnemyBullet(newBullet);

}

void Enemy::FireReset() {

	Fire();

	timeCalls_.push_back(new TimeCall(std::bind(&Enemy::FireReset,this),60));

}

void (Enemy::*Enemy::MoveFanction[])() = {
	&Enemy::ApproachingMove, 
	&Enemy::LeavingMove
};

void Enemy::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Enemy::OnCollision() { isDead_ = true; }

Enemy::~Enemy() {
	for (const auto& timeCall : timeCalls_) {
		delete timeCall;
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPosition{};

	worldPosition.x = worldTransform_.matWorld_.m[3][0];
	worldPosition.y = worldTransform_.matWorld_.m[3][1];
	worldPosition.z = worldTransform_.matWorld_.m[3][2];

	return worldPosition;
}