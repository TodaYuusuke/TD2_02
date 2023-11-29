#include "GameScene.h"
#include "StageSelectScene.h"

using namespace LWP::Utility;

GameScene::GameScene(int level, LWP::Math::Vector3 prevScenePosition) : level_(level) {
	prevScenePosition_ = prevScenePosition;
}

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

	// 常に表示するUI
	for (int i = 0; i < UI_NUM; i++) {
		// 実体を生成
		operationUI_[i] = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
		operationUI_[i]->isUI = true;
	}
	// ろうそくの数
	for (int i = 0; i < 4; i++) {
		// 実体を生成
		candleNumUI_[i] = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
		candleNumUI_[i]->isUI = false;
	}

	// 画像の張り替え
	operationUI_[RB]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_RB.png");
	operationUI_[LB]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_LB.png");
	operationUI_[UPBUTTON]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/crosskey.png");
	operationUI_[START]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_startButton.png");
	operationUI_[FONT_THROW]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_throw.png");
	operationUI_[FONT_PARALLEL]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_parallel.png");
	operationUI_[FONT_ZOOM]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_zoom.png");
	operationUI_[FONT_RESET]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_reset.png");
	operationUI_[TUTORIAL_CANDLE]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_lightFire.png");
	operationUI_[TUTORIAL_GROWLEAF]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_growGrass.png");
	operationUI_[TUTORIAL_WITHERLEAF]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_witherGrass.png");
	operationUI_[CANDLE]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/candle.png");
	operationUI_[X]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_x.png");
	// ろうそくの数
	candleNumUI_[0]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_1.png");
	candleNumUI_[1]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_2.png");
	candleNumUI_[2]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_3.png");
	candleNumUI_[3]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_4.png");

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
	// 上ボタン
	operationUI_[UPBUTTON]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[UPBUTTON]->vertices[1].position = { 1280.0f,0.0f,0.0f };
	operationUI_[UPBUTTON]->vertices[2].position = { 1280.0f,1280.0f,0.0f };
	operationUI_[UPBUTTON]->vertices[3].position = { 0.0f,1280.0f,0.0f };
	// スタートボタン
	operationUI_[START]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[START]->vertices[1].position = { 1280.0f,0.0f,0.0f };
	operationUI_[START]->vertices[2].position = { 1280.0f,1280.0f,0.0f };
	operationUI_[START]->vertices[3].position = { 0.0f,1280.0f,0.0f };
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
	// 文字_ズーム
	operationUI_[FONT_ZOOM]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[FONT_ZOOM]->vertices[1].position = { 200.0f,0.0f,0.0f };
	operationUI_[FONT_ZOOM]->vertices[2].position = { 200.0f,55.0f,0.0f };
	operationUI_[FONT_ZOOM]->vertices[3].position = { 0.0f,55.0f,0.0f };
	// 文字_リセット
	operationUI_[FONT_RESET]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[FONT_RESET]->vertices[1].position = { 600.0f,0.0f,0.0f };
	operationUI_[FONT_RESET]->vertices[2].position = { 600.0f,55.0f,0.0f };
	operationUI_[FONT_RESET]->vertices[3].position = { 0.0f,55.0f,0.0f };
	// ろうそく
	operationUI_[CANDLE]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[CANDLE]->vertices[1].position = { 1280.0f,0.0f,0.0f };
	operationUI_[CANDLE]->vertices[2].position = { 1280.0f,1280.0f,0.0f };
	operationUI_[CANDLE]->vertices[3].position = { 0.0f,1280.0f,0.0f };
	// 掛ける
	operationUI_[X]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[X]->vertices[1].position = { 60.0f,0.0f,0.0f };
	operationUI_[X]->vertices[2].position = { 60.0f,55.0f,0.0f };
	operationUI_[X]->vertices[3].position = { 0.0f,55.0f,0.0f };
	// チュートリアル用の文字
	if (level_ == 1) {
		// チュートリアル_ろうそく
		operationUI_[TUTORIAL_CANDLE]->vertices[0].position = { 0.0f,0.0f,0.0f };
		operationUI_[TUTORIAL_CANDLE]->vertices[1].position = { 720.0f,0.0f,0.0f };
		operationUI_[TUTORIAL_CANDLE]->vertices[2].position = { 720.0f,55.0f,0.0f };
		operationUI_[TUTORIAL_CANDLE]->vertices[3].position = { 0.0f,55.0f,0.0f };
	}
	if (level_ == 2 || level_ == 3) {
		// チュートリアル_成長する草
		operationUI_[TUTORIAL_GROWLEAF]->vertices[0].position = { 0.0f,0.0f,0.0f };
		operationUI_[TUTORIAL_GROWLEAF]->vertices[1].position = { 720.0f,0.0f,0.0f };
		operationUI_[TUTORIAL_GROWLEAF]->vertices[2].position = { 720.0f,55.0f,0.0f };
		operationUI_[TUTORIAL_GROWLEAF]->vertices[3].position = { 0.0f,55.0f,0.0f };
	}
	if (level_ == 4) {
		// チュートリアル_成長する草
		operationUI_[TUTORIAL_WITHERLEAF]->vertices[0].position = { 0.0f,0.0f,0.0f };
		operationUI_[TUTORIAL_WITHERLEAF]->vertices[1].position = { 720.0f,0.0f,0.0f };
		operationUI_[TUTORIAL_WITHERLEAF]->vertices[2].position = { 720.0f,55.0f,0.0f };
		operationUI_[TUTORIAL_WITHERLEAF]->vertices[3].position = { 0.0f,55.0f,0.0f };
	}
	// ろうそくの数
	for (int i = 0; i < 4; i++) {
		candleNumUI_[i]->vertices[0].position = { 0.0f,0.0f,0.0f };
		candleNumUI_[i]->vertices[1].position = { 60.0f,0.0f,0.0f };
		candleNumUI_[i]->vertices[2].position = { 60.0f,55.0f,0.0f };
		candleNumUI_[i]->vertices[3].position = { 0.0f,55.0f,0.0f };
	}

	// UIの座標と大きさ
	// RBボタン
	operationUI_[RB]->transform.translation = { 10, 10, 0 };
	operationUI_[RB]->transform.scale = { 0.4f, 0.4f, 1.0f };
	// LBボタン
	operationUI_[LB]->transform.translation = { 10, 70, 0 };
	operationUI_[LB]->transform.scale = { 0.4f, 0.4f, 1.0f };
	// 上ボタン
	operationUI_[UPBUTTON]->transform.translation = { 15, 130, 0 };
	operationUI_[UPBUTTON]->transform.scale = { 0.05f, 0.05f, 1.0f };
	// スタートボタン
	operationUI_[START]->transform.translation = { 20, 200, 0 };
	operationUI_[START]->transform.scale = { 0.04f, 0.04f, 1.0f };
	// 文字_投げる
	operationUI_[FONT_THROW]->transform.translation = { 100, 25, 0 };
	operationUI_[FONT_THROW]->transform.scale = { 0.6f, 0.6f, 1.0f };
	// 文字_平行移動
	operationUI_[FONT_PARALLEL]->transform.translation = { 100, 85, 0 };
	operationUI_[FONT_PARALLEL]->transform.scale = { 0.6f, 0.6f, 1.0f };
	// 文字_ズーム
	operationUI_[FONT_ZOOM]->transform.translation = { 100, 145, 0 };
	operationUI_[FONT_ZOOM]->transform.scale = { 0.6f, 0.6f, 1.0f };
	// 文字_リセット
	operationUI_[FONT_RESET]->transform.translation = { 100, 215, 0 };
	operationUI_[FONT_RESET]->transform.scale = { 0.5f, 0.5f, 1.0f };
	// チュートリアル_ろうそく
	operationUI_[TUTORIAL_CANDLE]->transform.translation = { 360, 20, 0 };
	// チュートリアル_成長する草
	operationUI_[TUTORIAL_GROWLEAF]->transform.translation = { 360, 20, 0 };
	// チュートリアル_枯れる草
	operationUI_[TUTORIAL_WITHERLEAF]->transform.translation = { 360, 20, 0 };
	// ろうそく
	operationUI_[CANDLE]->transform.translation = { 1100, 10, 0 };
	operationUI_[CANDLE]->transform.scale = { 0.05f, 0.05f, 1.0f };
	// 掛ける
	operationUI_[X]->transform.translation = { 1150, 10, 0 };
	// ろうそくの数
	for (int i = 0; i < 4; i++) {
		candleNumUI_[i]->transform.translation = { 1210, 10, 0 };
	}

	backButton_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	backButton_->isUI = true;
	backButton_->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_backButton.png");
	backButton_->vertices[0].position = { 0.0f,0.0f,0.0f };
	backButton_->vertices[1].position = { 1280.0f,0.0f,0.0f };
	backButton_->vertices[2].position = { 1280.0f,1280.0f,0.0f };
	backButton_->vertices[3].position = { 0.0f,1280.0f,0.0f };
	// UIの座標と大きさ
	// RBボタン
	backButton_->transform.translation = { 10, 640, 0 };
	backButton_->transform.scale = { 0.05f, 0.05f, 1.0f };

	// ステージセレクトに戻るボタン
	selectStageUI_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	selectStageUI_->isUI = true;
	selectStageUI_->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_stageSelect.png");
	selectStageUI_->vertices[0].position = { 0.0f,0.0f,0.0f };
	selectStageUI_->vertices[1].position = { 600.0f,0.0f,0.0f };
	selectStageUI_->vertices[2].position = { 600.0f,55.0f,0.0f };
	selectStageUI_->vertices[3].position = { 0.0f,55.0f,0.0f };
	// UIの座標と大きさ
	// RBボタン
	selectStageUI_->transform.translation = { 60, 660, 0 };
	selectStageUI_->transform.scale = { 0.6f, 0.6f, 1.0f };

	transition_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	transition_->vertices[0].position = { 0.0f,0.0f,0.0f };
	transition_->vertices[1].position = { 1280.0f,0.0f,0.0f };
	transition_->vertices[2].position = { 1280.0f,720.0f,0.0f };
	transition_->vertices[3].position = { 0.0f,720.0f,0.0f };
	transition_->commonColor = new Color(BLACK);	// 背景色をセット
	transition_->isUI = true;
}

void GameScene::Update() {
	// ステージ上に残っているろうそくの本数を調べる
	for (int i = 0; i < 4; i++) {
		if (stage_.GetCandleCount() - stage_.GetIgnitedCandle() == i + 1) {
			candleNumUI_[i]->isUI = true;
		}
		else {
			candleNumUI_[i]->isUI = false;
		}
	}

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
			next_ = new StageSelectScene(0xB4B4B4FF, prevScenePosition_);
			transition_->commonColor = new Color(0xB4B4B400);	// 色をセット
			isEnd_ = true;
		}

		// リスタート
		if (LWP::Input::Keyboard::GetTrigger(DIK_R) || LWP::Input::Pad::GetTrigger(0, XBOX_START) || player_.IsRetry()) {
			next_ = new GameScene(level_, prevScenePosition_);
			isEnd_ = true;
		}
		// 前のシーンへ戻る
		if (LWP::Input::Keyboard::GetTrigger(DIK_ESCAPE) || LWP::Input::Pad::GetTrigger(0, XBOX_BACK)) {
			next_ = new StageSelectScene(BLACK, prevScenePosition_);
			isEnd_ = true;
		}
	}
}