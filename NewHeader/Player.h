#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include <ImGuiManager.h>
#include <cassert>
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	void Update();
	void Draw(ViewProjection& viewProjection);

	void Rotate();
	void Attack();

	void OnCollision();

	void SetParent(const WorldTransform* parent);

	~Player();

	Vector3 GetWorldPosition();
	const std::list<PlayerBullet*>& GetBullets() { return playerBullets_; };

private:
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> playerBullets_;
};