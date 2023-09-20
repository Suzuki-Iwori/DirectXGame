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
#include "Particle.h"
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
	/// リスト達回す用
	/// </summary>
	void UpdateList();

	/// <summary>
	/// 敵の追加コマンド読み込み
	/// </summary>
	void LoadEnemyPopCommand();

	/// <summary>
	/// 敵の追加コマンド処理
	/// </summary>
	void UpdateEnemyPopCommand();

	/// <summary>
	/// スコア加算処理
	/// </summary>
	void AddScore() { score++; }

	/// <summary>
	/// 表示用スコア変換処理
	/// </summary>
	void ConvertScore();

	/// <summary>
	/// パーティクル生成関数
	/// </summary>
	void AddParticle(const uint32_t& num, const Vector3& position);

private: // メンバ変数

	/// <summary>
	/// 2つのコライダーの接触判定
	/// </summary>
	void CheckColliderPair(Collider* cA, Collider* cB);

	//システム関連
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	ViewProjection viewProjection_;

	std::stringstream enemyPopCommands_;
	bool isWaitingCommand = false;
	int waitingCommandTimer = 0;

	bool isGameEnd = false;

	uint32_t score = 0u;
	uint32_t scoreDigit[3]{};

	uint32_t resultFrame = 0u;

	float scoreAlpha = 1.0f;
	float backgroundAlpha = 0.0f;
	bool resultDisplay = false;

	//モデル
	Model* playeModel_ = nullptr;
	Model* skydomeModel_ = nullptr;
	Model* enemyModel_ = nullptr;
	Model* playerBulletModel_ = nullptr;
	Model* enemyBulletModel_ = nullptr;
	Model* particleModel_ = nullptr;

	//スプライトとテクスチャ
	uint32_t lifeTexture_ = 0u;
	Sprite* playerLifeImage_[4] = {nullptr};

	uint32_t numberTexture_[10] = {0u};
	Sprite* numberImage_[10][3] = {nullptr};

	uint32_t backgroundTexture_ = 0;
	Sprite* backgroundImage_ = nullptr;

	//オブジェクト
	Player* player_ = nullptr;
	std::list<Enemy*> enemy_;
	std::list<EnemyBullet*> enemyBullets_;
	std::list<Particle*> particles_;
	Skydome* skydome_ = nullptr;

	//カメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	RailCamera* railCamera_ = nullptr;	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};