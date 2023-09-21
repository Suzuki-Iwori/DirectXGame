#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete player_;
	delete playeModel_;
	delete debugCamera_;
	delete skydome_;
	delete skydomeModel_;
	delete playerBulletModel_;
	delete enemyBulletModel_;
	delete particleModel_;
	delete railCamera_;

	for (uint32_t i = 0; i < 3; i++) {
		delete playerLifeImage_[i];
	}

	for (uint32_t i = 0; i < 10; i++) {
		for (uint32_t j = 0; j < 3; j++) {
			delete numberImage_[i][j];
		}
	}

	delete blackoutImage_;
	delete restartImage_;

	for (const auto& enemy : enemy_) {
		delete enemy;
	}
	for (const auto& bullet : enemyBullets_) {
		delete bullet;
	}
	for (const auto& particle : particles_) {
		delete particle;
	}
}

void GameScene::Initialize() {

	//システム初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_.farZ = 11.0f;
	viewProjection_.Initialize();

	Vector3 playerPosition = {0.0f, 0.0f, 50.0f};

	score = 0u;
	resultFrame = 0u;
	scoreAlpha = 1.0f;
	blackoutAlpha = 0.0f;
	restartAlpha = 1.0f;
	isGameEnd = false;
	resultDisplay = false;

	//モデル初期化
	playeModel_ = Model::CreateFromOBJ("flyPlayer", true);
	playerBulletModel_ = Model::CreateFromOBJ("PBullet", true);
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	enemyModel_ = Model::CreateFromOBJ("enemii", true);
	particleModel_ = Model::CreateFromOBJ("cube", true);

	//スプライト初期化
	TextureManager::Load("target.png");

	lifeTexture_ = TextureManager::Load("texture/heart.png");
	for (uint32_t i = 0; i < 4; i++) {
		playerLifeImage_[i] = 
			Sprite::Create(lifeTexture_, {(10.0f + i * 74.0f), 10.0f}, {1, 1, 1, 1}, {0.0f, 0.0f});
	}

	numberTexture_[0] = TextureManager::Load("number/0.png");
	numberTexture_[1] = TextureManager::Load("number/1.png");
	numberTexture_[2] = TextureManager::Load("number/2.png");
	numberTexture_[3] = TextureManager::Load("number/3.png");
	numberTexture_[4] = TextureManager::Load("number/4.png");
	numberTexture_[5] = TextureManager::Load("number/5.png");
	numberTexture_[6] = TextureManager::Load("number/6.png");
	numberTexture_[7] = TextureManager::Load("number/7.png");
	numberTexture_[8] = TextureManager::Load("number/8.png");
	numberTexture_[9] = TextureManager::Load("number/9.png");

	for (uint32_t i = 0; i < 10; i++) {
		for (uint32_t j = 0; j < 3; j++) {
			numberImage_[i][j] =
			    Sprite::Create(numberTexture_[i], {(10.0f + j * 58.0f), 84.0f}, {1, 1, 1, 1}, {0.0f, 0.0f});
		}
	}

	blackoutTexture_ = TextureManager::Load("texture/Background.png");
	blackoutImage_ = Sprite::Create(blackoutTexture_, {0.0f, 0.0f}, {1, 1, 1, 0}, {0.0f, 0.0f});
	restartImage_ = Sprite::Create(blackoutTexture_, {0.0f, 0.0f}, {1, 1, 1, 0}, {0.0f, 0.0f});

	//オブジェクト初期化
	player_ = new Player;
	player_->Initialize(playeModel_, playerBulletModel_, playerPosition);

	skydome_ = new Skydome;
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera;
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	player_->SetParent(&railCamera_->GetWorldTransform());

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(false);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	enemyBulletModel_ = Model::CreateFromOBJ("enemyBullet", true);
	LoadEnemyPopCommand();

}

void GameScene::Update() {


	if (!isGameEnd) {
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

		if (!player_->GetPlayerLive()) {
			isGameEnd = true;
		}

		UpdateEnemyPopCommand();

		UpdateList();

		skydome_->Update();
		railCamera_->Update();

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER) {
				restartFrag = true;
			}
		}


	} else {
		resultFrame++;

		if (resultFrame >= 50) {
			resultDisplay = true;
		}

		if (!resultDisplay) {
			scoreAlpha -= 0.02f;
		} else {
			scoreAlpha += 0.02f;
			blackoutAlpha += 0.015f;
		}
		if (scoreAlpha > 1.0f) {
			scoreAlpha = 1.0f;
		}
		if (scoreAlpha < 0.0f) {
			scoreAlpha = 0.0f;
		}

		if (blackoutAlpha > 0.75f) {
			blackoutAlpha = 0.75f;
		}
		if (blackoutAlpha < 0.0f) {
			blackoutAlpha = 0.0f;
		}

		for (uint32_t i = 0; i < 10; i++) {
			for (uint32_t j = 0; j < 3; j++) {

				if (scoreDigit[j] == i) {

					numberImage_[i][j]->SetColor({1.0f, 1.0f, 1.0f, scoreAlpha});

					if (resultDisplay) {
						numberImage_[i][j]->SetPosition({568.0f + j * 58.0f, 328.0f});
					}

				}
			}
		}


		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER ||
			    joyState.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER) {
				if (resultDisplay) {
					restartFrag = true;
				}
			}
		}

	}


	if (restartFrag) {
		Restart();
	}


	if (restartAlpha > 0.0f && !restartFrag) {
		restartAlpha -= 0.02f;
	}


	blackoutImage_->SetColor({1.0f, 1.0f, 1.0f, blackoutAlpha});
	restartImage_->SetColor({1.0f, 1.0f, 1.0f, restartAlpha});

	CheckAllCollisions();

	ConvertScore();

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

	for (const auto& particle : particles_) {
		particle->Draw(viewProjection_);
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

	for (uint32_t i = 0; i < player_->GetPlayerLife(); i++) {
		playerLifeImage_[i]->Draw();
	}

	blackoutImage_->Draw();

	for (uint32_t i = 0; i < 10; i++) {
		for (uint32_t j = 0; j < 3; j++) {

			if (scoreDigit[j] == i) {

				numberImage_[i][j]->Draw();

			}

		}
	}

	restartImage_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ConvertScore() {

	scoreDigit[0] = score / 100u;
	scoreDigit[1] = (score % 100u) / 10;
	scoreDigit[2] = (score % 100u) % 10;

}

void GameScene::AddParticle(const uint32_t& num, const Vector3& position) {

	for (uint32_t i = 0; i < num; i++) {

		Particle* newParticle = new Particle;
		newParticle->Initialize(particleModel_, position);

		particles_.push_back(newParticle);

	}

}

void GameScene::Restart() {

	restartFrame++;

	restartAlpha += 0.04f;

	if (restartFrame == 25) {

		Vector3 playerPosition = {0.0f, 0.0f, 50.0f};

		score = 0u;
		resultFrame = 0u;
		scoreAlpha = 1.0f;
		blackoutAlpha = 0.0f;
		isGameEnd = false;
		resultDisplay = false;
		restartFrame = 0u;
		restartFrag = false;

		// スプライト初期化
		for (uint32_t i = 0; i < 10; i++) {
			for (uint32_t j = 0; j < 3; j++) {
				numberImage_[i][j]->SetPosition({(10.0f + j * 58.0f), 84.0f});
			}
		}
		blackoutImage_->SetColor({1, 1, 1, 0});

		// オブジェクト初期化
		player_->Restart({playerPosition});

		railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});

		enemy_.clear();
		enemyBullets_.clear();
		particles_.clear();

		// スクリプト初期化
		enemyPopCommands_.str("");
		enemyPopCommands_.clear(std::stringstream::goodbit);

		LoadEnemyPopCommand();

	}

}

void GameScene::CheckColliderPair(Collider* cA, Collider* cB) {
	Vector3 posA = cA->GetWorldPosition();
	Vector3 posB = cB->GetWorldPosition();

	float length = sqrtf(
	(posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
	(posA.z - posB.z) * (posA.z - posB.z));

	if (length <= (cA->GetRadius() + cB->GetRadius())) {
	
		cA->OnCollision();
		cB->OnCollision();
	
	}

}

void GameScene::CheckAllCollisions() {
	Vector3 PosA{}, PosB{};

	const std::list<PlayerBullet*> playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*> enemyBullets = enemyBullets_;

	for (const auto& bullet : enemyBullets) {
	
		CheckColliderPair(player_, bullet);

	}

	for (const auto& enemy : enemy_) {

		for (PlayerBullet* bullet : playerBullets) {

			CheckColliderPair(enemy, bullet);

		}
	}

	for (const auto& enemyBullet : enemyBullets) {

		for (PlayerBullet* playerBullet : playerBullets) {

			CheckColliderPair(enemyBullet, playerBullet);

		}
	}

}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	enemyBullets_.push_back(enemyBullet);

}

void GameScene::AddEnemy(const Vector3& position) {

	Enemy* newEnemy = new Enemy;
	newEnemy->Initialize(enemyModel_, enemyBulletModel_, position, {0.0f, 0.0f, -0.5f});
	newEnemy->SetPlayer(player_);
	newEnemy->SetGameScene(this);

	enemy_.push_back(newEnemy);

}

void GameScene::UpdateList() {

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

		if (bullet->IsDestroyed()) {
			AddParticle(3, bullet->GetWorldPosition());
			score++;
		}
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead() || bullet->IsDestroyed()) {
			delete bullet;
			return true;
		} else {
			return false;
		}
	});

	for (const auto& particle : particles_) {
		particle->Update();
	}

	particles_.remove_if([](Particle* particle) {
		if (particle->IsDead()) {
			delete particle;
			return true;
		} else {
			return false;
		}
	});

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

		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWaitingCommand = true;
			waitingCommandTimer = waitTime;

			break;
		} else if (word.find("END") == 0) {
			isGameEnd = true;
		}
	}
}