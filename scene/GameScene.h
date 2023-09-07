#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>
#include <DebugCamera.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 接触判定
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾の追加
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵の追加
	/// </summary>
	void AddEnemy(const Vector3& position);

	/// <summary>
	/// 敵の追加コマンド読み込み
	/// </summary>
	void LoadEnemyPopCommand();

	/// <summary>
	/// 敵の追加コマンド処理
	/// </summary>
	void UpdateEnemyPopCommand();

private: // メンバ変数


	/// <summary>
	/// 2つのコライダーの接触判定
	/// </summary>
	void CheckColliderPair(Collider* cA, Collider* cB);


	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	std::list<Enemy*> enemy_;
	std::list<EnemyBullet*> enemyBullets_;
	Skydome* skydome_ = nullptr;
	Model* skydomeModel_ = nullptr;

	Model* enemyModel_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	std::stringstream enemyPopCommands_;
	bool isWaitingCommand = false;
	int waitingCommandTimer = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};