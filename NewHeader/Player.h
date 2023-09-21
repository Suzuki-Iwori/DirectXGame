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
	void Initialize(Model* model, Model* bulletModel, const Vector3& position);

	void Update(ViewProjection& viewProjection);
	void Draw(ViewProjection& viewProjection);

	void Move();
	void Attack();
	void SetShot();

	void Restart(const Vector3& position);

	void OnCollision() override;

	void SetParent(const WorldTransform* parent);

	void TransformUI(ViewProjection& viewProjection);
	void DrawUI();

	~Player();

	Vector3 GetWorldPosition() override;
	const std::list<PlayerBullet*>& GetBullets() { return playerBullets_; };
	uint32_t GetPlayerLife() { return playerLife_; }
	bool GetPlayerLive() { return isPlayerLive_; }

private:
	uint32_t textureHandle_ = 0u;
	uint32_t bulletCoolTime_ = 0;

	uint32_t playerLife_ = 4;
	bool isPlayerLive_ = true;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;

	Model* bulletModel_ = nullptr;
	std::list<PlayerBullet*> playerBullets_;

	WorldTransform worldTransform3DReticle_;
	Sprite* sprite2DReticle_ = nullptr;

};