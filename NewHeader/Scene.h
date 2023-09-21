#pragma once

enum class SceneList {
	Scene_Title,
	Scene_GamePlay, 
	Scene_Num
};

class Scene {
public:
	static bool sceneTransition[(unsigned int)SceneList::Scene_Num]; //シーン切り替えフラグ(静的)

	Scene();
	virtual ~Scene() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SceneFragReset();

};