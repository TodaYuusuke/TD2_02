#include "GameScene.h"

GameScene::GameScene(int level) : level_(level) {}

void GameScene::Initialize() {
	stage_.Init(level_);
	lantern_.Init();
}

void GameScene::Update() {
	stage_.Update();
	lantern_.Update();
}