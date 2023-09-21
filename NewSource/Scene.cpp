#include "Scene.h"

Scene::Scene() {
	for (int i = 0; i < (unsigned int)SceneList::Scene_Num; i++) {
		sceneTransition[i] = false;
	}
}

void Scene::SceneFragReset() {
	for (int i = 0; i < (unsigned int)SceneList::Scene_Num; i++) {
		sceneTransition[i] = false;
	}
}