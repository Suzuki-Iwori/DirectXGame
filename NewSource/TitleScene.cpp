#include "TitleScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

Title::Title() {}

void Title::Initialize() {

	// システム初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_.farZ = 11.0f;
	viewProjection_.Initialize();

	//スプライト初期化
	blackoutTexture_ = TextureManager::Load("texture/Background.png");
	blackoutImage_ = Sprite::Create(blackoutTexture_, {0.0f, 0.0f}, {1, 1, 1, 0}, {0.0f, 0.0f});
	backgroundTexture_ = TextureManager::Load("texture/TitleBackground.png");
	backgroundImage_ = Sprite::Create(backgroundTexture_, {0.0f, 0.0f}, {1, 1, 1, 1}, {0.0f, 0.0f});

	RBTexture_ = TextureManager::Load("texture/RB.png");
	RBImage_ = Sprite::Create(RBTexture_, {380.0f, 470.0f}, {1, 1, 1, 1}, {0.0f, 0.5f});
	LBTexture_ = TextureManager::Load("texture/LB.png");
	LBImage_ = Sprite::Create(LBTexture_, {380.0f, 610.0f}, {1, 1, 1, 1}, {0.0f, 0.5f});

	startLetterTexture_ = TextureManager::Load("texture/start.png");
	startLetterImage_ = Sprite::Create(startLetterTexture_, {550.0f, 470.0f}, {1, 1, 1, 1}, {0.0f, 0.5f});
	manualLetterTexture_ = TextureManager::Load("texture/manualLetter.png");
	manualLetterImage_ = Sprite::Create(manualLetterTexture_, {550.0f, 610.0f}, {1, 1, 1, 1}, {0.0f, 0.5f});

	manualTexture_ = TextureManager::Load("texture/manual.png");
	manualImage_ = Sprite::Create(manualTexture_, {0.0f, 0.0f}, {1, 1, 1, 0}, {0.0f, 0.0f});

}

void Title::Update() {

	if (startFlag) {
		titleFrame++;

		blackoutAlpha += 0.04f;
	}
	if (titleFrame > 25) {
		sceneTransition[(unsigned int)SceneList::Scene_GamePlay] = true;
	}

	if (manualFlag) {
		manualAlpha = 1.0f;
	} else {
		manualAlpha = 0.0f;
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) {

			startFlag = true;
		}
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER && manualCoolTime == 0) {

			manualFlag = !manualFlag;
			manualCoolTime = 40;
		}
	}

	manualCoolTime--;
	if (manualCoolTime < 0) {
		manualCoolTime = 0;
	}

	blackoutImage_->SetColor({1.0f, 1.0f, 1.0f, blackoutAlpha});
	manualImage_->SetColor({1.0f, 1.0f, 1.0f, manualAlpha});
}

void Title::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	backgroundImage_->Draw();
	RBImage_->Draw();
	LBImage_->Draw();
	startLetterImage_->Draw();
	manualLetterImage_->Draw();
	

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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	manualImage_->Draw();
	blackoutImage_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

Title::~Title() {

	delete blackoutImage_;
	delete backgroundImage_;
	delete RBImage_;
	delete LBImage_;
	delete startLetterImage_;
	delete manualLetterImage_;
	delete manualImage_;

}
