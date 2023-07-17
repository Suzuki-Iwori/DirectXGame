#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete debugCamera_;
	delete skydome_;
	delete skydomeModel_;
	delete railCamera_;

	for (const auto& enemy : enemy_) {
		delete enemy;
	}
	for (const auto& bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("white1x1.png");
	model_ = Model::Create();

	viewProjection_.farZ = 11.0f;
	viewProjection_.Initialize();

	Vector3 playerPosition = {0.0f, 0.0f, 50.0f};

	TextureManager::Load("target.png");

	player_ = new Player;
	player_->Initialize(model_, textureHandle_, playerPosition);

	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome;
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera;
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	player_->SetParent(&railCamera_->GetWorldTransform());

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	LoadEnemyPopCommand();

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

	player_->Update(viewProjection_);

	UpdateEnemyPopCommand();

	for (const auto& enemy : enemy_) {
		enemy->Update();
	}

	enemy_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		} else {
			return false;
		}
	});

	for (const auto& bullet : enemyBullets_) {
		bullet->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		} else {
			return false;
		}
	});


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

	for (const auto& enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}

	for (const auto& bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
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

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 PosA{}, PosB{};

	PosA = player_->GetWorldPosition();

	const std::list<PlayerBullet*> playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*> enemyBullets = enemyBullets_;

	for (const auto& bullet : enemyBullets) {
	
		PosB = bullet->GetWorldPosition();

		float length = sqrtf(
		    (PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y) +
		    (PosA.z - PosB.z) * (PosA.z - PosB.z));


		if (length <= 2.0f) {
			player_->OnCollision();
			bullet->OnCollision();
		}

	}

	for (const auto& enemy : enemy_) {

		PosA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {

			PosB = bullet->GetWorldPosition();

			float length = sqrtf(
			    (PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y) +
			    (PosA.z - PosB.z) * (PosA.z - PosB.z));

			if (length <= 2.0f) {
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}

	for (const auto& enemyBullet : enemyBullets) {

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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	enemyBullets_.push_back(enemyBullet);

}

void GameScene::AddEnemy(const Vector3& position) {

	Enemy* newEnemy = new Enemy;
	newEnemy->Initialize(model_, position, {0.0f, 0.0f, -0.5f});
	newEnemy->SetPlayer(player_);
	newEnemy->SetGameScene(this);

	enemy_.push_back(newEnemy);

}

void GameScene::LoadEnemyPopCommand() {

	std::ifstream file;
	file.open("./Resources/PopEnemyScript.csv");
	assert(file.is_open());

	enemyPopCommands_ << file.rdbuf();

	file.close();

}

void GameScene::UpdateEnemyPopCommand() {

	if (isWaitingCommand) {
		waitingCommandTimer--;
		if (waitingCommandTimer <= 0) {
			isWaitingCommand = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands_, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {

			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			AddEnemy({x, y, z});

		}
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWaitingCommand = true;
			waitingCommandTimer = waitTime;

			break;
		}
	}

}