#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include <ImGuiManager.h>
#include <cassert>
#include "PlayerBullet.h"
#include "Sprite.h"
#include <list>

class Player : public Collider {
public:
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	void Update(ViewProjection& viewProjection);
	void Draw(ViewProjection& viewProjection);

	void Rotate();
	void Attack();

	void OnCollision() override;

	void SetParent(const WorldTransform* parent);

	void TransformUI(ViewProjection& viewProjection);
	void DrawUI();

	~Player();

	Vector3 GetWorldPosition() override;
	const std::list<PlayerBullet*>& GetBullets() { return playerBullets_; };

private:
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> playerBullets_;

	WorldTransform worldTransform3DReticle_;

	Sprite* sprite2DReticle_ = nullptr;

};