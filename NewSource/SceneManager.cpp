#include "sceneManager.h"
#include "scene.h"
#include "GameScene.h"
#include "TitleScene.h"

SceneManager::SceneManager() {

	HaveScene_[(unsigned int)SceneList::Scene_Title] = new Title;
	HaveScene_[(unsigned int)SceneList::Scene_GamePlay] = new GameScene;

	OperationScene_ = (unsigned int)SceneList::Scene_Title;

}

// 初期化
void SceneManager::Initialize() {
	for (unsigned int i = 0; i < (unsigned int)SceneList::Scene_Num; i++) {
			HaveScene_[i]->Initialize();
	}
}
// 更新
void SceneManager::Update() {
	for (unsigned int i = 0; i < (unsigned int)SceneList::Scene_Num; i++) {
		if (OperationScene_ == i) {
			HaveScene_[i]->Update();
		}

		if (sceneTransition[i]) {
			OperationScene_ = i;
			SceneFragReset();
			HaveScene_[i]->Initialize();
		}
	}
}
// 描写
void SceneManager::Draw() {
	for (unsigned int i = 0; i < (unsigned int)SceneList::Scene_Num; i++) {
		if (OperationScene_ == i) {
			HaveScene_[i]->Draw();
		}
	}
}
SceneManager::~SceneManager() {
	for (unsigned int i = 0; i < (unsigned int)SceneList::Scene_Num; i++) {
		delete HaveScene_[i];
	}
}