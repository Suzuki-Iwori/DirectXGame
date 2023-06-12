#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete skydome_;
	delete skydomeModel_;
	delete railCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("./Resources/white1x1.png");
	model_ = Model::Create();

	viewProjection_.farZ = 11.0f;
	viewProjection_.Initialize();

	Vector3 playerPosition = {0.0f, 0.0f, 20.0f};

	player_ = new Player;
	player_->Initialize(model_, textureHandle_, playerPosition);

	enemy_ = new Enemy;
	enemy_->Initialize(model_, {20.0f, 0.0f, 180.0f}, {0.0f, 0.0f, -0.5f});
	enemy_->SetPlayer(player_);

	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome;
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera;
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	player_->SetParent(&railCamera_->GetWorldTransform());

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {


	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {

		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

	}

	player_->Update();

	if (enemy_ != nullptr) {
		enemy_->Update();	
	}

	CheckAllCollisions();
	skydome_->Update();

	railCamera_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skydome_->Draw(viewProjection_);

	player_->Draw(viewProjection_);

	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 PosA{}, PosB{};

	PosA = player_->GetWorldPosition();

	const std::list<PlayerBullet*> playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*> enemyBullets = enemy_->GetBullets();

	for (EnemyBullet* bullet : enemyBullets) {
	
		PosB = bullet->GetWorldPosition();

		float length = sqrtf(
		    (PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y) +
		    (PosA.z - PosB.z) * (PosA.z - PosB.z));


		if (length <= 2.0f) {
			player_->OnCollision();
			bullet->OnCollision();
		}

	}

	PosA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {

		PosB = bullet->GetWorldPosition();

		float length = sqrtf(
		    (PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y) +
		    (PosA.z - PosB.z) * (PosA.z - PosB.z));

		if (length <= 2.0f) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}

	for (EnemyBullet* enemyBullet : enemyBullets) {

		PosA = enemyBullet->GetWorldPosition();

		for (PlayerBullet* playerBullet : playerBullets) {

			PosB = playerBullet->GetWorldPosition();

			float length = sqrtf(
			    (PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y) +
			    (PosA.z - PosB.z) * (PosA.z - PosB.z));

			if (length <= 2.0f) {
				enemyBullet->OnCollision();
				playerBullet->OnCollision();
			}
		}
	}

}