#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	input_ = Input::GetInstance();

}
void Player::Update() {

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

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.rotation_.x += move.z;
	worldTransform_.rotation_.y += move.x;
	worldTransform_.rotation_.z += move.y;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kLimitY);

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	ImGui::Begin("worldTransform");
	ImGui::Text(
	    "%8.4f %8.4f %8.4f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();




	



}
void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}