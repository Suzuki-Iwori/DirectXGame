#pragma once
#include "Scene.h"

class SceneManager : public Scene {
private:
	unsigned int OperationScene_; // 稼働シーン

	Scene* HaveScene_[(unsigned int)SceneList::Scene_Num]; // シーン

public:
	SceneManager();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描写
	void Draw() override;

	~SceneManager() override;
};