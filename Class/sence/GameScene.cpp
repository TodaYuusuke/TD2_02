#include "GameScene.h"
#include "StageSelectScene.h"

using namespace LWP::Utility;

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

	// 音の再生(ループ再生)
	for (int i = 0; i < 3; i++) {
		bgm_[i]->SetLoopCount(255);
		bgm_[i]->Play();
		bgm_[i]->SetVolume(0.00f);
	}
	bgm_[1]->SetVolume(0.00f);

	transition_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	transition_->vertices[0].position = { 0.0f,0.0f,0.0f };
	transition_->vertices[1].position = { 1280.0f,0.0f,0.0f };
	transition_->vertices[2].position = { 1280.0f,720.0f,0.0f };
	transition_->vertices[3].position = { 0.0f,720.0f,0.0f };
	transition_->commonColor = new Color(BLACK);	// 背景色をセット
	transition_->isUI = true;
}

void GameScene::Update() {
	// トランジション更新
	if (isStart_) {
		stage_.Update();
		player_.Update(&stage_);
		transitionFrame_--;
		transition_->commonColor->A = static_cast<char>(static_cast<int>(easeOut((float)(transitionFrame_) / 60.0f, 0.0f, 255.0f, 30.0f / 60.0f)));

		if (transitionFrame_ <= 0) {
			isStart_ = false;
		}
	}
	else if (isEnd_) {
		transitionFrame_++;
		transition_->commonColor->A = static_cast<char>(static_cast<int>(easeOut((float)(transitionFrame_) / 60.0f, 0.0f, 255.0f, 30.0f / 60.0f)));

		if (transitionFrame_ >= 30) {
			nextScene_ = next_;
		}
	}
	else {
		stage_.Update();
		player_.Update(&stage_);

		// クリアチェック
		if (stage_.IsCleared()) {
			next_ = new StageSelectScene(0xB4B4B4FF);
			transition_->commonColor = new Color(0xB4B4B400);	// 色をセット
			isEnd_ = true;
		}

		// リスタート
		if (LWP::Input::Keyboard::GetTrigger(DIK_R)) {
			next_ = new GameScene(level_);
			isEnd_ = true;
		}
		// 前のシーンへ戻る
		if (LWP::Input::Keyboard::GetTrigger(DIK_ESCAPE)) {
			next_ = new StageSelectScene(BLACK);
			isEnd_ = true;
		}
	}
}