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

	// 常に表示するUI
	for (int i = 0; i < UI_NUM; i++) {
		// 実体を生成
		operationUI_[i] = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
		operationUI_[i]->isUI = true;
	}
	// 画像の張り替え
	operationUI_[RB]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_RB.png");
	operationUI_[LB]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_LB.png");
	operationUI_[FONT_THROW]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_throw.png");
	operationUI_[FONT_PARALLEL]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_parallel.png");
	// 平面の頂点座標を設定
	// RBボタン
	operationUI_[RB]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[RB]->vertices[1].position = { 200.0f,0.0f,0.0f };
	operationUI_[RB]->vertices[2].position = { 200.0f,120.0f,0.0f };
	operationUI_[RB]->vertices[3].position = { 0.0f,120.0f,0.0f };
	// LBボタン
	operationUI_[LB]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[LB]->vertices[1].position = { 200.0f,0.0f,0.0f };
	operationUI_[LB]->vertices[2].position = { 200.0f,120.0f,0.0f };
	operationUI_[LB]->vertices[3].position = { 0.0f,120.0f,0.0f };
	// 文字_投げる
	operationUI_[FONT_THROW]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[FONT_THROW]->vertices[1].position = { 200.0f,0.0f,0.0f };
	operationUI_[FONT_THROW]->vertices[2].position = { 200.0f,55.0f,0.0f };
	operationUI_[FONT_THROW]->vertices[3].position = { 0.0f,55.0f,0.0f };
	// 文字_平行移動
	operationUI_[FONT_PARALLEL]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[FONT_PARALLEL]->vertices[1].position = { 200.0f,0.0f,0.0f };
	operationUI_[FONT_PARALLEL]->vertices[2].position = { 200.0f,55.0f,0.0f };
	operationUI_[FONT_PARALLEL]->vertices[3].position = { 0.0f,55.0f,0.0f };

	// UIの座標と大きさ
	// RBボタン
	operationUI_[RB]->transform.translation = { 10, 10, 0 };
	operationUI_[RB]->transform.scale = { 0.4f, 0.4f, 1.0f };
	// LBボタン
	operationUI_[LB]->transform.translation = { 10, 70, 0 };
	operationUI_[LB]->transform.scale = { 0.4f, 0.4f, 1.0f };
	// 文字_投げる
	operationUI_[FONT_THROW]->transform.translation = { 100, 25, 0 };
	operationUI_[FONT_THROW]->transform.scale = { 0.6f, 0.6f, 1.0f };
	// 文字_平行移動
	operationUI_[FONT_PARALLEL]->transform.translation = { 100, 85, 0 };
	operationUI_[FONT_PARALLEL]->transform.scale = { 0.6f, 0.6f, 1.0f };

	stage_.Init(level_);
	player_.Init(stage_.GetPlayerStartPosition(), mainCamera);



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