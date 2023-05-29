#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
#include "EnemyBullet.h"
#include <list>

class Player;

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velosity);

	void Update();

	void ApproachingMove();
	void LeavingMove();

	void ApproachingInitialize();
	void Fire();

	void Draw(const ViewProjection& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

	void OnCollision();

	~Enemy();

	Vector3 GetWorldPosition();
	const std::list<EnemyBullet*>& GetBullets() { return enemyBullets_; };

private:

	enum class Phase {
		Approach,
		Leave,
	};

	static void (Enemy::*MoveFanction[])();

	const int32_t kFireInterval = 60;

	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velosity_{};
	Phase phase_ = Phase::Approach;
	std::list<EnemyBullet*> enemyBullets_;
	int32_t fireCount = 0;

	Player* player_ = nullptr;

};
