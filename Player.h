#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include <ImGuiManager.h>
#include <cassert>
#include "PlayerBullet.h"

class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Rotate();

	void Attack();

	~Player();

private:
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	PlayerBullet* playerBullet_ = nullptr;
};