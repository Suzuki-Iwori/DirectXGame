#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate) {

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;

	viewProjection_.Initialize();

}

void RailCamera::Update() {

	Vector3 move = {0.0f, 0.0f, 0.03f};
	Vector3 rot = {0.0f, 0.0f, 0.0f};

	worldTransform_.translation_ += move;
	worldTransform_.rotation_ += rot;

	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

}
