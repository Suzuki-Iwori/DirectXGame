#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	input_ = Input::GetInstance();

}
void Player::Update() {

	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		} else {
			return false;
		}
	});

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	const float kLimitX = 33.0f;
	const float kLimitY = 17.0f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	Rotate();

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kLimitY);

	worldTransform_.UpdateMatrix();

	Attack();

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}

}
void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection);
	}

}
void Player::Rotate() {

	const float kRotateSpeed = 0.02f;	

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotateSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotateSpeed;
	}

}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float bulletSpeed = 1.0f;
		Vector3 velosity(0.0f, 0.0f, bulletSpeed);

		velosity = TransformNormal(velosity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet;
		newBullet->Initialize(model_, GetWorldPosition(), velosity);

		playerBullets_.push_back(newBullet);

	}
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

Player::~Player() { 
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPosition{};

	worldPosition.x = worldTransform_.matWorld_.m[3][0];
	worldPosition.y = worldTransform_.matWorld_.m[3][1];
	worldPosition.z = worldTransform_.matWorld_.m[3][2];

	return worldPosition;
}