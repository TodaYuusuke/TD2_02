#include "GameScene.h"
#include "StageSelectScene.h"

GameScene::GameScene(int level) : level_(level) {}

void GameScene::Initialize() {
	backGround_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	backGround_->vertices[0].position = { 0.0f,0.0f,1000.0f };
	backGround_->vertices[1].position = { 1280.0f,0.0f,1000.0f };
	backGround_->vertices[2].position = { 1280.0f,720.0f,1000.0f };
	backGround_->vertices[3].position = { 0.0f,720.0f,1000.0f };
#if _DEBUG
	backGround_->commonColor = new LWP::Utility::Color(LWP::Utility::WHITE);	// 背景色をセット
#else
	backGround_->commonColor = new LWP::Utility::Color(LWP::Utility::BLACK);	// 背景色をセット
#endif
	backGround_->isUI = true;

	stage_.Init(level_);
	player_.Init(stage_.GetPlayerStartPosition(), mainCamera);

	// 音の読み込み
	bgm_[0] = LWP::Resource::LoadAudio("BGM_playStage.wav");
	bgm_[1] = LWP::Resource::LoadAudio("BGM_bird.wav");
	bgm_[2] = LWP::Resource::LoadAudio("BGM_leafWindow.wav");

	bgm_[0]->SetVolume(0.05f);
	bgm_[2]->SetVolume(0.05f);
	// 音の再生(ループ再生)
	for (int i = 0; i < 3; i++) {
		bgm_[i]->SetLoopCount(255);
		bgm_[i]->Play();
	}
}

void GameScene::Update() {
	stage_.Update();
	player_.Update(&stage_);

	// 再読み込み
	if (LWP::Input::Keyboard::GetTrigger(DIK_R)) {
		nextScene_ = new GameScene(level_);
	}
	// 前のシーンへ戻る
	if (LWP::Input::Keyboard::GetTrigger(DIK_ESCAPE)) {
		nextScene_ = new StageSelectScene();
	}
}