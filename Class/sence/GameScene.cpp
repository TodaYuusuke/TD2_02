#include "GameScene.h"

GameScene::GameScene(int level) : level_(level) {}

void GameScene::Initialize() {
	stage_.Init(level_);
	player_.Init(mainCamera);
}

void GameScene::Update() {
	stage_.Update();
	player_.Update(&stage_);

	// 再読み込み
	if (LWP::Input::Keyboard::GetTrigger(DIK_R)) {
		nextScene_ = new GameScene(level_);
	}
}