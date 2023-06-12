#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
}
void Skydome::Update() {

	worldTransform_.UpdateMatrix();

}
void Skydome::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}