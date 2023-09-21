#pragma once
#include "Scene.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <sstream>
#include <DebugCamera.h>

class Title : public Scene {

public:
	Title();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描写
	void Draw() override;

	~Title();

private:

	XINPUT_STATE joyState{};

	// システム関連
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	ViewProjection viewProjection_;

	uint32_t titleFrame = 0u;

	float blackoutAlpha = 0.0f;
	bool startFlag = false;

	int manualCoolTime = 0;
	float manualAlpha = 0.0f;
	bool manualFlag = false;

	// スプライトとテクスチャ
	uint32_t blackoutTexture_ = 0;
	Sprite* blackoutImage_ = nullptr;
	uint32_t backgroundTexture_ = 0;
	Sprite* backgroundImage_ = nullptr;

	uint32_t RBTexture_ = 0;
	Sprite* RBImage_ = nullptr;
	uint32_t LBTexture_ = 0;
	Sprite* LBImage_ = nullptr;

	uint32_t startLetterTexture_ = 0;
	Sprite* startLetterImage_ = nullptr;
	uint32_t manualLetterTexture_ = 0;
	Sprite* manualLetterImage_ = nullptr;

	uint32_t manualTexture_ = 0;
	Sprite* manualImage_ = nullptr;

};