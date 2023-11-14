#include "GameScene.h"

GameScene::GameScene(int level) : level_(level) {}

void GameScene::Initialize() {
	stage_.Init(level_);
	player_.Init(mainCamera);
}

void GameScene::Update() {
	stage_.Update();
	player_.Update(&stage_);
}