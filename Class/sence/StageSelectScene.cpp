#include "StageSelectScene.h"
#include "GameScene.h"

using namespace LWP::Utility;

StageSelectScene::StageSelectScene(Color startTransitionColor) {
	transitionColor_ = startTransitionColor;
}

StageSelectScene::StageSelectScene(LWP::Utility::Color startTransitionColor, LWP::Math::Vector3 startPosition) {
	transitionColor_ = startTransitionColor;
	isSelectStartPosition_ = true;
	startPosition_ = startPosition;
}

void StageSelectScene::Initialize() {
	backGround_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	backGround_->vertices[0].position = { 0.0f,0.0f,1000.0f };
	backGround_->vertices[1].position = { 1280.0f,0.0f,1000.0f };
	backGround_->vertices[2].position = { 1280.0f,720.0f,1000.0f };
	backGround_->vertices[3].position = { 0.0f,720.0f,1000.0f };
#if _DEBUG
	backGround_->commonColor = new LWP::Utility::Color(WHITE);	// 背景色をセット
#else
	backGround_->commonColor = new LWP::Utility::Color(BLACK);	// 背景色をセット
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
	operationUI_[UPBUTTON]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/crosskey.png");
	operationUI_[FONT_START]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_start.png");
	operationUI_[FONT_PARALLEL]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_parallel.png");
	operationUI_[FONT_ZOOM]->texture = LWP::Resource::LoadTextureLongPath("resources/UI/UI_zoom.png");

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
	// 文字_スタート
	operationUI_[FONT_START]->vertices[0].position = { 0.0f,0.0f,0.0f };
	operationUI_[FONT_START]->vertices[1].position = { 600.0f,0.0f,0.0f };
	operationUI_[FONT_START]->vertices[2].position = { 600.0f,55.0f,0.0f };
	operationUI_[FONT_START]->vertices[3].position = { 0.0f,55.0f,0.0f };
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
	// 文字_スタート
	operationUI_[FONT_START]->transform.translation = { 95, 25, 0 };
	operationUI_[FONT_START]->transform.scale = { 0.5f, 0.5f, 1.0f };
	// 文字_平行移動
	operationUI_[FONT_PARALLEL]->transform.translation = { 100, 85, 0 };
	operationUI_[FONT_PARALLEL]->transform.scale = { 0.6f, 0.6f, 1.0f };
	// 文字_ズーム
	operationUI_[FONT_ZOOM]->transform.translation = { 100, 145, 0 };
	operationUI_[FONT_ZOOM]->transform.scale = { 0.6f, 0.6f, 1.0f };

	stageSelect_.Init(&player_);
	if (isSelectStartPosition_) {
		player_.Init(startPosition_, mainCamera);
	}
	else {
		player_.Init(stageSelect_.GetPlayerStartPosition(), mainCamera);
	}

	transition_ = LWP::Primitive::CreateInstance<LWP::Primitive::Surface>();
	transition_->vertices[0].position = { 0.0f,0.0f,0.0f };
	transition_->vertices[1].position = { 1280.0f,0.0f,0.0f };
	transition_->vertices[2].position = { 1280.0f,720.0f,0.0f };
	transition_->vertices[3].position = { 0.0f,720.0f,0.0f };
	transition_->commonColor = new Color(transitionColor_);	// 背景色をセット
	transition_->isUI = true;
}
// 更新
void StageSelectScene::Update() {

	// トランジション更新
	if (isStart_) {
		stageSelect_.Update();
		player_.Update(&stageSelect_);
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
		stageSelect_.Update();
		player_.Update(&stageSelect_);

		// 再読み込み
		if (LWP::Input::Keyboard::GetTrigger(DIK_R)) {
			next_ = new StageSelectScene(BLACK);
			transition_->commonColor = new Color(BLACK);	// 背景色をセット
			isEnd_ = true;
		}
		// もしステージが選択されたならステージを進める
		int level = player_.GetSelectedStageLevel();
		if (level != -1) {
			next_ = new GameScene(level, player_.GetWorldPosition());
			transition_->commonColor = new Color(BLACK);	// 背景色をセット
			isEnd_ = true;
		}
	}
}